#include "Manager.h"

Manager::Manager() {}

Manager::~Manager() {}

bool Manager::init(int numPriorityLevels, std::vector<int> totalResources) {
    runningProcess = -1;

    readyList = PriorityRL(numPriorityLevels);

    resources.clear();
    processMap.clear();

    for (int i = 0; i < totalResources.size(); ++i) {
        // Create a RCB with id = nextResourceID and
        // inventory = totalResources[i]
        auto newResource = std::make_shared<RCB>(i, totalResources[i]);
        int resourceId = resources.insert(newResource);
        if (resourceId == -1) {
            return false;
        }
    }

    // Create the init process with priority = 0
    auto initProcess = std::make_shared<PCB>(0);
    initProcess->state = ProcessState::RUNNING;
    int processId = processMap.insert(initProcess);
    if (processId == -1) {
        return false;
    } else {
        initProcess->id = processId;
    }
    readyList.insertProcess(initProcess);
    runningProcess = processId;

    return true;
}

bool Manager::init_default() { return init(3, {1, 1, 2, 3}); }

bool Manager::create(int priority) {
    if (priority < 1) {
        return false;
    }

    // Create a PCB with priority = priority
    auto newProcess = std::make_shared<PCB>(priority);
    int id = processMap.insert(newProcess);
    if (id == -1) {
        return false;
    } else {
        newProcess->id = id;
    }
    newProcess->state = ProcessState::READY;
    readyList.insertProcess(newProcess);

    // update child list of current running process
    auto parent = readyList.getRunningProcess();
    parent->children.push_back(newProcess);
    // update parent of new process
    newProcess->parent = parent;

    scheduler();
    return true;
}

bool Manager::destroy(int processID) {
    if (!processMap.exists(processID)) {
        return false; // Process does not exist
    }

    auto runningProcess = readyList.getRunningProcess();
    // Check if the process to be destroyed is a child of the running process
    auto &children = runningProcess->children;
    if (std::find_if(children.begin(), children.end(),
                     [processID](const std::shared_ptr<PCB> &child) {
                         return child->id == processID;
                     }) == children.end()) {
        return false;
    }

    std::queue<int> queue;
    queue.push(processID);

    while (!queue.empty()) {
        int currentID = queue.front();
        queue.pop();

        auto process = processMap.get(currentID);
        if (process == nullptr) {
            continue;
        }

        // Push all children to queue
        for (auto &child : process->children) {
            queue.push(child->id);
        }

        // Remove from parent's child list
        if (process->parent) {
            auto &siblings = process->parent->children;
            for (auto it = siblings.begin(); it != siblings.end();) {
                if ((*it)->id == currentID) {
                    it = siblings.erase(it);
                } else {
                    ++it;
                }
            }
        }

        // release all resources of process
        for (auto &res : process->resources) {
            release(res.second, res.first->id, process->id);
        }

        readyList.removeProcess(process->id);

        // remove from map
        processMap.remove(currentID);
    }
    return true;
}

bool Manager::request(int units, int resourceID) {
    if (!resources.exists(resourceID)) {
        return false;
    }
    auto resource = resources.get(resourceID);

    // Current running process
    auto process = readyList.getRunningProcess();

    // Prevent process 0 from requesting any resources
    // and requesting 0 units
    if (runningProcess == 0 || units < 1) {
        return false;
    }
    // Check if the units requested have exceeded the inventory
    if (resource->inventory < units) {
        return false;
    }

    // Check if free units are available
    if (resource->state >= units) {
        // Add the resource to the running process's resource list
        resource->state -= units;
        process->resources.push_back(std::make_pair(resource, units));
        return true;
    } else {
        process->state = ProcessState::BLOCKED;
        readyList.removeProcess(process->id);
        resource->waitlist.emplace_back(process, units);
        return scheduler();
    }
}

bool Manager::release(int units, int resourceID, int processID) {
    if (!resources.exists(resourceID)) {
        return false;
    }
    auto resource = resources.get(resourceID);

    std::shared_ptr<PCB> process;
    if (processID == -1) {
        // set process to running process if not specified
        process = readyList.getRunningProcess();
    } else {
        if (!processMap.exists(processID)) {
            return false;
        }
        process = processMap.get(processID);
    }

    // Check if the process actually holds the resource it's trying to
    // release
    int totalUnitsHeld = 0;
    for (auto &res : process->resources) {
        if (res.first->id == resourceID) {
            totalUnitsHeld += resource->inventory - res.first->state;
        }
    }
    if (totalUnitsHeld < units) {
        return false;
    }

    // remove (r, k) from i.resources
    process->resources.erase(
        std::remove_if(process->resources.begin(), process->resources.end(),
                       [resourceID, units](
                           const std::pair<std::shared_ptr<RCB>, int> &rcb) {
                           return rcb.first->id == resourceID &&
                                  rcb.second == units;
                       }),
        process->resources.end());

    resource->state += units;
    while (resource->waitlist.empty() == false && resource->state > 0) {
        auto &[blockedProcess, blockedUnits] = resource->waitlist.front();
        if (resource->state >= blockedUnits) {
            // Allocate resource to the blocked process
            resource->state -= blockedUnits;
            blockedProcess->state = ProcessState::READY;
            blockedProcess->resources.push_back(
                std::make_pair(resource, blockedUnits));

            // Remove the process from the resource's waitlist
            resource->waitlist.pop_front();

            // Add the process back to the ready list
            readyList.insertProcess(blockedProcess);
        } else {
            break;
        }
    }
    return scheduler();
}

bool Manager::scheduler() {
    // Get the process with the highest priority
    auto nextProcess = readyList.getHighestPriorityProcess();
    if (nextProcess == nullptr) {
        return false;
    }

    // Update the running process
    runningProcess = nextProcess->id;
    readyList.contextSwitch();
    return true;
}

bool Manager::timeout() {
    // Get the current running process
    auto process = readyList.getRunningProcess();
    if (process == nullptr) {
        return false;
    }

    // Move the process to the end of the queue
    readyList.removeProcess(process->id);
    readyList.insertProcess(process);

    return scheduler();
}

int Manager::executeCommand(const std::string &command) {
    std::istringstream stream(command);
    std::string cmd;
    stream >> cmd;
    bool result = false;
    // std::cout << command << std::endl;

    if (cmd == "in") {
        int n, u0, u1, u2, u3;
        stream >> n >> u0 >> u1 >> u2 >> u3;
        result = init(n, {u0, u1, u2, u3});
    } else if (cmd == "id") {
        result = init_default();
    } else if (cmd == "cr") {
        int priority;
        stream >> priority;
        result = create(priority);
    } else if (cmd == "de") {
        int processID;
        stream >> processID;
        result = destroy(processID);
    } else if (cmd == "rq") {
        int units, resourceID;
        stream >> resourceID >> units;
        result = request(units, resourceID);
    } else if (cmd == "rl") {
        int units, resourceID;
        stream >> resourceID >> units;
        result = release(units, resourceID);
    } else if (cmd == "to") {
        result = timeout();
    }
    if (!result) {
        return -1;
    }
    return runningProcess;
}
#include "Manager.h"

Manager::Manager() {}

Manager::~Manager() {}

bool Manager::init(int numPriorityLevels, std::vector<int> totalResources) {
    runningProcess = -1;
    nextProcessID = 0;
    nextResourceID = 0;

    readyList = PriorityRL(numPriorityLevels);

    resources.clear();
    processMap.clear();

    for (int i = 0; i < totalResources.size(); ++i) {
        // Create a RCB with id = nextResourceID and
        // inventory = totalResources[i]
        auto newResource =
            std::make_shared<RCB>(nextResourceID, totalResources[i]);
        resources[nextResourceID] = newResource;
        nextResourceID++;
    }

    // Create the init process with id = 0, priority = 0
    auto initProcess = std::make_shared<PCB>(nextProcessID, 0);
    initProcess->state = ProcessState::RUNNING;
    readyList.insertProcess(initProcess);
    processMap[nextProcessID] = initProcess;
    nextProcessID++;
    runningProcess = 0;

    return true;
}

bool Manager::init_default() { return init(3, {1, 1, 2, 3}); }

bool Manager::create(int priority) {
    if (priority < 1) {
        return false;
    }

    // Create a PCB with id = nextProcessID and priority = priority
    auto newProcess = std::make_shared<PCB>(nextProcessID, priority);
    newProcess->state = ProcessState::READY;
    readyList.insertProcess(newProcess);

    // update child list of current running process
    auto parent = readyList.getRunningProcess();
    parent->children.push_back(newProcess);
    // update parent of new process
    newProcess->parent = parent;

    // add to map
    processMap[nextProcessID] = newProcess;

    nextProcessID++;
    scheduler();
    return true;
}

bool Manager::destroy(int processID) {
    std::stack<int> stack;
    stack.push(processID);

    while (!stack.empty()) {
        int currentID = stack.top();
        stack.pop();

        auto it = processMap.find(currentID);
        if (it == processMap.end()) {
            return false;
        }

        auto process = it->second;

        // Push all children to stack
        for (auto &child : process->children) {
            stack.push(child->id);
        }

        // Remove from parent's child list
        if (process->parent) {
            auto &siblings = process->parent->children;
            siblings.erase(
                std::remove_if(siblings.begin(), siblings.end(),
                               [currentID](const std::shared_ptr<PCB> &pcb) {
                                   return pcb->id == currentID;
                               }),
                siblings.end());
        }

        // release resources
        for (auto &rcb : process->resources) {
            rcb->state += 1;
            release(1, rcb->id);
        }
        readyList.removeProcess(process->id);

        // remove from map
        processMap.erase(currentID);
    }
    return true;
}

bool Manager::request(int units, int resourceID) {
    auto it = resources.find(resourceID);
    if (it == resources.end()) {
        std::cerr << "Error: Resource " << resourceID << " does not exist."
                  << std::endl;
        return false;
    }
    auto resource = it->second;

    // Current running process
    auto process = readyList.getRunningProcess();

    // Prevent process 0 from requesting any resources
    if (runningProcess == 0) {
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
        process->resources.push_back(resource);
        return true;
    } else {
        process->state = ProcessState::BLOCKED;
        readyList.removeProcess(process->id);
        resource->waitlist.emplace_back(process, units);
        return scheduler();
    }
}

bool Manager::release(int units, int resourceID) {
    auto it = resources.find(resourceID);
    if (it == resources.end()) {
        return false;
    }
    auto resource = it->second;

    // Current running process
    auto process = readyList.getRunningProcess();

    // remove (process, units) from process->resources
    process->resources.erase(
        std::remove_if(process->resources.begin(), process->resources.end(),
                       [resourceID](const std::shared_ptr<RCB> &rcb) {
                           return rcb->id == resourceID;
                       }),
        process->resources.end());

    resource->state += units;
    while (resource->waitlist.empty() == false && resource->state > 0) {
        auto &[blockedProcess, blockedUnits] = resource->waitlist.front();
        if (resource->state >= blockedUnits) {
            // Allocate resource to the blocked process
            resource->state -= blockedUnits;
            blockedProcess->state = ProcessState::READY;
            blockedProcess->resources.push_back(resource);

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
        stream >> units >> resourceID;
        result = release(units, resourceID);
    } else if (cmd == "to") {
        result = timeout();
    }
    if (!result) {
        return -1;
    }
    return runningProcess;
}
#include "Manager.h"

void Manager::init(int numPriorityLevels, std::vector<int> totalResources) {
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
}

void Manager::init_default() { init(3, {1, 1, 2, 3}); }

void Manager::create(int priority) {
    if (priority < 1) {
        // TODO: add result to output
        return;
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
}

void Manager::destroy(int processID) {
    std::stack<int> stack;
    stack.push(processID);

    while (!stack.empty()) {
        int currentID = stack.top();
        stack.pop();

        auto it = processMap.find(currentID);
        if (it == processMap.end()) {
            std::cerr << "Process not found" << std::endl;
            continue;
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

        // TODO: release resources

        readyList.removeProcess(process->priority);

        // remove from map
        processMap.erase(currentID);
    }
}

void Manager::request(int units, int resourceID) {
    auto it = resources.find(resourceID);
    if (it == resources.end()) {
        // TODO: add result to output
        return;
    }
    auto resource = it->second;

    // Current running process
    auto process = readyList.getRunningProcess();

    // Check if free units are available
    if (resource->state >= units) {
        // Add the resource to the running process's resource list
        resource->state -= units;

        process->resources.push_back(resource);
    } else {
        // Mark the current running process as blocked
        process->state = ProcessState::BLOCKED;

        // Move process from readyList to waitlist of resource
        resource->waitlist.push(process);
        readyList.removeProcess(process->priority);

        scheduler();
    }
}

void Manager::release(int units, int resourceID) {
    auto it = resources.find(resourceID);
    if (it == resources.end()) {
        // TODO: add result to output
        return;
    }
    auto resource = it->second;

    // Current running process
    auto process = readyList.getRunningProcess();

    if (resource->state + units > resource->inventory) {
        // TODO: add result to output
        return;
    }

    // Remove the resource from the running process's resource list
    resource->state += units;

    process->resources.erase(
        std::remove_if(process->resources.begin(), process->resources.end(),
                       [resourceID](const std::shared_ptr<RCB> &rcb) {
                           return rcb->id == resourceID;
                       }),
        process->resources.end());

    // Check if waitlist is not empty
    if (!resource->waitlist.empty()) {
        // Move process from head of waitlist to readyList
        auto blockedProcess = resource->waitlist.front();
        resource->waitlist.pop();
        blockedProcess->state = ProcessState::READY;
        readyList.insertProcess(blockedProcess);

        // Insert resource to blocked process's resource list
        blockedProcess->resources.push_back(resource);

        scheduler();
    }
}

int Manager::scheduler() {
    // Get the process with the highest priority
    auto nextProcess = readyList.getHighestPriorityProcess();
    if (nextProcess == nullptr) {
        return -1;
    }

    // Update the running process
    runningProcess = nextProcess->id;
    readyList.contextSwitch();
    return runningProcess;
}

void Manager::timeout() {
    // Get the current running process
    auto process = readyList.getRunningProcess();
    if (process == nullptr) {
        // TODO: add result to output
        return;
    }

    // Move the process to the end of the queue
    readyList.removeProcess(process->priority);
    readyList.insertProcess(process);

    scheduler();
}

void Manager::executeCommand(const std::string &command) {
    std::istringstream stream(command);
    std::string cmd;
    stream >> cmd;

    if (cmd == "in") {
        int n, u0, u1, u2, u3;
        stream >> n >> u0 >> u1 >> u2 >> u3;
        init(n, {u0, u1, u2, u3});
    } else if (cmd == "id") {
        init_default();
    } else if (cmd == "cr") {
        int priority;
        stream >> priority;
        create(priority);
    } else if (cmd == "de") {
        int processID;
        stream >> processID;
        destroy(processID);
    } else if (cmd == "rq") {
        int units, resourceID;
        stream >> units >> resourceID;
        request(runningProcess, resourceID);
    } else if (cmd == "rl") {
        int units, resourceID;
        stream >> units >> resourceID;
        release(runningProcess, resourceID);
    } else if (cmd == "to") {
        timeout();
    }
    std::cout << runningProcess << std::endl;
}
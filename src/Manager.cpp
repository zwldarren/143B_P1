#include "Manager.h"

void Manager::init(int numPriorityLevels,
                   std::vector<int> resourceInventories) {
    runningProcess = -1;
    nextProcessID = 0;
    nextResourceID = 0;

    readyList = PriorityRL(numPriorityLevels);

    resources.clear();
    processMap.clear();

    for (int i = 0; i < resourceInventories.size(); ++i) {
        // Create a RCB with id = nextResourceID and
        // inventory = resourceInventories[i]
        nextResourceID++;
        resources.emplace_back(nextResourceID, resourceInventories[i]);
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
    }
    std::cout << runningProcess << std::endl;
}
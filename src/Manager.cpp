#include "Manager.h"

Manager::Manager(int numPriorityLevels, std::vector<int> resourceInventories) {
    init(numPriorityLevels, resourceInventories);
}

void Manager::init(int numPriorityLevels,
                   std::vector<int> resourceInventories) {

    readyList = PriorityRL(numPriorityLevels);

    resources.clear();

    for (int i = 0; i < resourceInventories.size() - 1; ++i) {
        // Create a RCB with id = i and inventory = resourceInventories[i]
        resources.emplace_back(i, resourceInventories[i]);
    }

    processes.clear();
    // No process running initially
    runningProcess = -1;
}

void Manager::init_default() { init(3, {1, 1, 2, 3}); }

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
    }
}

void Manager::create(int priority) {
    int nextID = 1;
    processes.emplace_back(nextID, priority);
    readyList.insertProcess(nextID, priority);
}
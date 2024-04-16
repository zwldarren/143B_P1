#include "Manager.h"
#include <PriorityRL.h>
#include <iostream>
#include <sstream>

Manager::Manager(int numPriorityLevels, std::vector<int> resourceInventories) {
    init(numPriorityLevels, resourceInventories);
}

void Manager::init(int numPriorityLevels,
                   std::vector<int> resourceInventories) {

    readyList = PriorityRL(numPriorityLevels);

    resources.clear();

    for (int i = 0; i < resourceInventories.size(); ++i) {
        resources.emplace_back(i, resourceInventories[i]);
    }

    processes.clear();
    // No process running initially
    runningProcess = -1;
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
        init(3, {1, 1, 2, 3}); // Default values
    }
}
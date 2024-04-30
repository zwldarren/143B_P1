#pragma once

#include "ArrayMap.h"
#include "PCB.h"
#include "PriorityRL.h"
#include "RCB.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

class Manager {
  private:
    // Map of process ID to PCB pointer
    ArrayMap<std::shared_ptr<PCB>, 16> processMap;
    // Map of resource ID to RCB pointer
    ArrayMap<std::shared_ptr<RCB>, 4> resources;
    PriorityRL readyList;
    int runningProcess; // Index of the currently running process

  public:
    Manager();
    ~Manager();

    // Initialize all data structures
    bool init(int numPriorityLevels, std::vector<int> resourceInventories);
    bool init_default();

    bool create(int priority);   // Create a new process
    bool destroy(int processID); // Destroy a process and its descendants
    bool request(int units, int resourceID); // Request a resource
    // Release a resource
    bool release(int units, int resourceID, int processID = -1);

    bool scheduler();
    bool timeout(); // Simulate a time-sharing timeout

    // Parse and execute commands
    int executeCommand(const std::string &command);
};
#pragma once

#include "PCB.h"
#include "PriorityRL.h"
#include "RCB.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

class Manager {
  private:
    // Map of process ID to PCB pointer
    std::unordered_map<int, std::shared_ptr<PCB>> processMap;
    // Map of resource ID to RCB pointer
    std::unordered_map<int, std::shared_ptr<RCB>> resources;
    PriorityRL readyList;
    int runningProcess; // Index of the currently running process
    int nextProcessID;  // ID to assign to the next process
    int nextResourceID; // ID to assign to the next resource

  public:
    Manager();
    ~Manager();

    // Initialize all data structures
    bool init(int numPriorityLevels, std::vector<int> resourceInventories);
    bool init_default();

    bool create(int priority);   // Create a new process
    bool destroy(int processID); // Destroy a process and its descendants
    bool request(int units, int resourceID); // Request a resource
    bool release(int units, int resourceID); // Release a resource

    bool scheduler();
    bool timeout(); // Simulate a time-sharing timeout

    // Parse and execute commands
    int executeCommand(const std::string &command);
};
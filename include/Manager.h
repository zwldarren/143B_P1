#pragma once

#include "PCB.h"
#include "PriorityRL.h"
#include "RCB.h"
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

class Manager {
  private:
    std::list<PCB> processes;
    std::list<RCB> resources;
    PriorityRL readyList;
    int runningProcess; // Index of the currently running process

  public:
    Manager() {}
    Manager(int numPriorityLevels, std::vector<int> resourceInventories);
    ~Manager() {}

    // Initialize all data structures
    void init(int numPriorityLevels, std::vector<int> resourceInventories);
    void init_default();

    void create(int priority);   // Create a new process
    void destroy(int processID); // Destroy a process and its descendants
    void request(int processID, int resourceID); // Request a resource
    void release(int processID, int resourceID); // Release a resource
    void timeout(); // Simulate a time-sharing timeout

    // Parse and execute commands
    void executeCommand(const std::string &command);
};
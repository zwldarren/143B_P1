#pragma once

#include "PCB.h"
#include "RCB.h"
#include <string>
#include <vector>

class Manager {
  public:
    Manager(int numProcesses, int numResources);
    ~Manager();

    void init(); // Initialize all data structures
    void createProcess(int processID, int parentID); // Create a new process
    void destroyProcess(int processID); // Destroy a process and its descendants
    void requestResource(int processID, int resourceID); // Request a resource
    void releaseResource(int processID, int resourceID); // Release a resource
    void timeout(); // Simulate a time-sharing timeout

    // Parse and execute commands
    void executeCommand(const std::string &command);

  private:
    std::vector<PCB> processes;
    std::vector<RCB> resources;
    int runningProcess; // Index of the currently running process
};
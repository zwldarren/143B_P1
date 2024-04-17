#pragma once
#include "PCB.h"
#include <array>
#include <memory>
#include <queue>

class PriorityRL {
  private:
    // 3 queues for 3 priorities
    std::vector<std::queue<std::shared_ptr<PCB>>> queues;

  public:
    PriorityRL() {}
    PriorityRL(int numPriorityLevels);

    // Add a process to a specific priority level
    void insertProcess(std::shared_ptr<PCB> process);

    // Remove the process at the front of the specified priority level
    int removeProcess(int priority);

    // Get the next process to run
    std::shared_ptr<PCB> getHighestPriorityProcess() const;

    // Get current running process
    std::shared_ptr<PCB> getRunningProcess() const;

    void contextSwitch();
};
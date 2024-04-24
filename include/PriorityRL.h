#pragma once
#include "PCB.h"
#include <array>
#include <list>
#include <memory>

class PriorityRL {
  private:
    // 3 lists for 3 priorities
    std::vector<std::list<std::shared_ptr<PCB>>> lists;

  public:
    PriorityRL() {}
    PriorityRL(int numPriorityLevels);

    // Add a process to a specific priority level
    void insertProcess(std::shared_ptr<PCB> process);

    // Remove the process by id
    void removeProcess(int id);

    // Get the next process to run
    std::shared_ptr<PCB> getHighestPriorityProcess();

    // Get current running process
    std::shared_ptr<PCB> getRunningProcess();

    void contextSwitch();
};
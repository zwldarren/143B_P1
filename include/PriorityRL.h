#pragma once
#include <array>
#include <queue>

class PriorityRL {
  private:
    // 3 queues for 3 priorities
    std::vector<std::queue<int>> queues;

  public:
    PriorityRL() {}
    PriorityRL(int numPriorityLevels);

    // Add a process to a specific priority level
    void insertProcess(int processId, int priority);

    // Remove the process at the front of the specified priority level
    int removeProcess(int priority);
};
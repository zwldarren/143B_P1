#pragma once
#include <array>
#include <queue>

class PriorityRL {
  private:
    // 3 queues for 3 priorities
    std::array<std::queue<int>, 3> queues;

  public:
    PriorityRL() {}

    // Add a process to a specific priority level
    void insertProcess(int processId, int priority);

    // Remove the process at the front of the specified priority level
    int removeProcess(int priority);
};
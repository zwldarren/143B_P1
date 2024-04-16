#pragma once
#include <vector>

enum class ProcessState { READY, RUNNING, BLOCKED };

struct PCB {
    int id;
    ProcessState state;
    int parent;
    int priority;
    std::vector<int> children;
    std::vector<int> resources;

    PCB(int id, int priority)
        : id(id), state(ProcessState::READY), priority(priority) {}
    PCB(int id, int parent_id, int priority)
        : id(id), state(ProcessState::READY), parent(parent_id),
          priority(priority) {}
};

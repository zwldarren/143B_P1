#pragma once
#include <memory>
#include <vector>

enum class ProcessState { READY, RUNNING, BLOCKED };

struct PCB {
    int id;
    ProcessState state;
    std::shared_ptr<PCB> parent;
    int priority;
    std::vector<std::shared_ptr<PCB>> children;
    std::vector<int> resources;

    PCB(int id, int priority)
        : id(id), state(ProcessState::READY), priority(priority) {}
    PCB(int id, std::shared_ptr<PCB> parent, int priority)
        : id(id), state(ProcessState::READY), parent(parent),
          priority(priority) {}
};

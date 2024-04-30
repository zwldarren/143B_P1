#pragma once
#include <memory>
#include <utility>
#include <vector>

struct RCB;
enum class ProcessState { READY, RUNNING, BLOCKED };

struct PCB {
    int id;
    ProcessState state;
    std::shared_ptr<PCB> parent;
    int priority;
    std::vector<std::shared_ptr<PCB>> children;
    // vector of pairs(resource, units)
    std::vector<std::pair<std::shared_ptr<RCB>, int>> resources;

    PCB(int priority)
        : id(-1), state(ProcessState::READY), priority(priority) {}
    PCB(int id, int priority)
        : id(id), state(ProcessState::READY), priority(priority) {}
};

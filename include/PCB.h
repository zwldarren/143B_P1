#pragma once
#include "RCB.h"
#include <memory>
#include <vector>

enum class ProcessState { READY, RUNNING, BLOCKED };

struct PCB {
    int id;
    ProcessState state;
    std::shared_ptr<PCB> parent;
    int priority;
    std::vector<std::shared_ptr<PCB>> children;
    std::vector<std::shared_ptr<RCB>> resources;

    PCB(int id, int priority)
        : id(id), state(ProcessState::READY), priority(priority) {}
};

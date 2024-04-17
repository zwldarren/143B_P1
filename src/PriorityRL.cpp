#include "PriorityRL.h"

PriorityRL::PriorityRL(int numPriorityLevels) {
    queues.resize(numPriorityLevels);
}

void PriorityRL::insertProcess(std::shared_ptr<PCB> process) {
    queues[process->priority].push(process);
}

int PriorityRL::removeProcess(int priority) {
    if (priority < 0 || priority >= queues.size() || queues[priority].empty()) {
        return -1;
    }
    std::shared_ptr<PCB> process = queues[priority].front();
    queues[priority].pop();
    return process->id;
}

std::shared_ptr<PCB> PriorityRL::getHighestPriorityProcess() const {
    for (int i = queues.size() - 1; i > -1; i--) {
        if (!queues[i].empty()) {
            return queues[i].front();
        }
    }
    return nullptr;
}

std::shared_ptr<PCB> PriorityRL::getRunningProcess() const {
    for (int i = queues.size() - 1; i > -1; i--) {
        if (!queues[i].empty() &&
            queues[i].front()->state == ProcessState::RUNNING) {
            return queues[i].front();
        }
    }
    return nullptr;
}

void PriorityRL::contextSwitch() {
    // Change the state of the RUNNING process to READY
    for (int i = queues.size() - 1; i > -1; i--) {
        if (!queues[i].empty() &&
            queues[i].front()->state == ProcessState::RUNNING) {
            queues[i].front()->state = ProcessState::READY;
            break;
        }
    }
    // Change the highest priority READY process to RUNNING
    auto process = getHighestPriorityProcess();
    process->state = ProcessState::RUNNING;
}
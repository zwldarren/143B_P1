#include "PriorityRL.h"

PriorityRL::PriorityRL(int numPriorityLevels) {
    lists.resize(numPriorityLevels);
}

void PriorityRL::insertProcess(std::shared_ptr<PCB> process) {
    lists[process->priority].push_back(process);
}

void PriorityRL::removeProcess(int id) {
    for (auto it = lists.begin(); it != lists.end(); ++it) {
        for (auto process = it->begin(); process != it->end(); ++process) {
            if ((*process)->id == id) {
                it->erase(process);
                return;
            }
        }
    }
}

std::shared_ptr<PCB> PriorityRL::getHighestPriorityProcess() {
    for (int i = lists.size() - 1; i > -1; i--) {
        if (!lists[i].empty()) {
            return lists[i].front();
        }
    }
    return nullptr;
}

std::shared_ptr<PCB> PriorityRL::getRunningProcess() {
    for (int i = lists.size() - 1; i > -1; i--) {
        if (!lists[i].empty() &&
            lists[i].front()->state == ProcessState::RUNNING) {
            return lists[i].front();
        }
    }
    return nullptr;
}

/*
    Context Switch will change the state of the RUNNING process to READY and
    change the state of the highest priority READY process to RUNNING
*/
void PriorityRL::contextSwitch() {
    // Change the state of the RUNNING process to READY
    for (auto it = lists.begin(); it != lists.end(); ++it) {
        if (!it->empty()) {
            for (auto process : *it) {
                if (process->state == ProcessState::RUNNING) {
                    process->state = ProcessState::READY;
                    break;
                }
            }
        }
    }
    // Change the highest priority READY process to RUNNING
    auto process = getHighestPriorityProcess();
    process->state = ProcessState::RUNNING;
}
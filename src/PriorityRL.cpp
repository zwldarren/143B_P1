#include "PriorityRL.h"

PriorityRL::PriorityRL(int numPriorityLevels) {
    queues.resize(numPriorityLevels);
}

void PriorityRL::insertProcess(int processId, int priority) {
    queues[priority].push(processId);
}

int PriorityRL::removeProcess(int priority) {
    if (priority < 0 || priority >= queues.size()) {
        return -1;
    }
    if (queues[priority].empty()) {
        return -1;
    }
    int processId = queues[priority].front();
    queues[priority].pop();
    return processId;
}

int PriorityRL::getHighestPriorityProcess() const {
    for (int i = 0; i < queues.size(); ++i) {
        if (!queues[i].empty()) {
            return queues[i].front();
        }
    }
    return -1;
}

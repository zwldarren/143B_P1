#include "PriorityRL.h"

PriorityRL::PriorityRL(int numPriorityLevels) {
    queues.resize(numPriorityLevels);
}
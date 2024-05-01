#include "PCB.h"
#include "RCB.h"

void PCB::removeFromChildren(int processID) {
    for (auto it = children.begin(); it != children.end();) {
        if ((*it)->id == processID) {
            it = children.erase(it);
        } else {
            ++it;
        }
    }
}

bool PCB::isChild(int processID) {
    for (auto child : children) {
        if (child->id == processID) {
            return true;
        }
    }
    return false;
}

void PCB::resourceRelease(int resourceID) {
    for (auto it = resources.begin(); it != resources.end();) {
        if (it->first->id == resourceID) {
            it = resources.erase(it);
        } else {
            ++it;
        }
    }
}
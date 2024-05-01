#include "RCB.h"
#include "PCB.h"

void RCB::removeFromWaitlist(int processID) {
    for (auto it = waitlist.begin(); it != waitlist.end();) {
        if (it->first->id == processID) {
            it = waitlist.erase(it);
        } else {
            ++it;
        }
    }
}
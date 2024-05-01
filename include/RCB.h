#pragma once
#include <list>
#include <memory>
#include <utility>

struct PCB;

struct RCB {
    int id;
    int state; // current available units
    std::list<std::pair<std::shared_ptr<PCB>, int>> waitlist;
    int inventory; // initial number of units

    RCB(int inventory) : id(-1), state(inventory), inventory(inventory) {}
    RCB(int id, int inventory)
        : id(id), state(inventory), inventory(inventory) {}

    void removeFromWaitlist(int processID);
};
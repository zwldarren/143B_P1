#pragma once
#include <memory>
#include <queue>

struct PCB;

struct RCB {
    int id;
    int state; // current available units
    std::queue<std::shared_ptr<PCB>> waitlist;
    int inventory; // initial number of units

    RCB(int id, int inventory)
        : id(id), state(inventory), inventory(inventory) {}
};
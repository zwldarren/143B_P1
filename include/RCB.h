#pragma once
#include <queue>

struct RCB {
    int id;
    int units; // current available units
    std::queue<int> waitlist;
    int inventory; // initial number of units

    RCB(int id, int inventory)
        : id(id), units(inventory), inventory(inventory) {}
};
#pragma once
#include "PCB.h"
#include <list>

class Scheduler {
  public:
    Scheduler() {}

    void schedule();
    void timeout();
};

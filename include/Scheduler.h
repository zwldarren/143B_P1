#pragma once
#include "PCB.h"
#include "PriorityRL.h"
#include <list>

class Scheduler {
  private:
    PriorityRL readyList;

  public:
    Scheduler() {}
    Scheduler(PriorityRL &rl) : readyList(rl) {}

    void schedule();
    void timeout();
    int scheduleNext();
};

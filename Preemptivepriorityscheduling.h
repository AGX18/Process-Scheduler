#pragma once
#include "scheduler.h"

class PreemptivePriorityScheduler : public Scheduler {
    Q_OBJECT

public:
    void schedule() override;

signals:
    void averagesCalculated(double avgTurnaroundTime, double avgWaitingTime);
};

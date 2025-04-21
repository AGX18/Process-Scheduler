#ifndef PREEMPTIVEPRIORITYSCHEDULING_H
#define PREEMPTIVEPRIORITYSCHEDULING_H

#pragma once
#include "scheduler.h"
#include "mainwindow.h"
#include "process.h"
#include <QTimer>
#include <deque>

class PreemptivePriorityScheduling {
public:
    PreemptivePriorityScheduling(int n);
    ~PreemptivePriorityScheduling();

    void run();

private:
    int n;
    int i;
    std::vector<Process*> processes;
    int completed;
    int time;
    float total_waiting_time;
    float total_turnaround_time;


    Process* getHighestPriorityProcess();
};
#endif // PREEMPTIVEPRIORITYSCHEDULING_H

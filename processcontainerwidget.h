#ifndef PREEMPTIVEPRIORITYSCHEDULING_H
#define PREEMPTIVEPRIORITYSCHEDULING_H

#include <vector>
#include "process.h"

class PreemptivePriorityScheduling {
public:
    PreemptivePriorityScheduling(int n);
    ~PreemptivePriorityScheduling();

    void setProcesses(const std::vector<Process*>& new_processes);
    void run();

private:
    int n; // عدد العمليات
    int completed; // عدد العمليات اللي خلصت
    int time; // الوقت الحالي
    int total_waiting_time;
    int total_turnaround_time;
    std::vector<Process*> processes;

    Process* getHighestPriorityProcess();
};

#endif // PREEMPTIVEPRIORITYSCHEDULING_H

#ifndef PREEMPTIVEPRIORITYSCHEDULING_H
#define PREEMPTIVEPRIORITYSCHEDULING_H

#include "Scheduler.h"
#include "Process.h"
#include <vector>

class PreemptivePriorityScheduler : public Scheduler
{
    Q_OBJECT

public:
    PreemptivePriorityScheduler(QObject *parent, std::vector<Process> Processes);
    ~PreemptivePriorityScheduler() = default;

    void schedule();
    void sortProcessesByPriority();
    void checkForArrival();
    void updateWaitingTimes();

private:
    int currentTime=0;
    int totalWaitingTime=0;
    int totalTurnaroundTime=0;
    int completedProcesses=0;
};

#endif // PREEMPTIVEPRIORITYSCHEDULING_H

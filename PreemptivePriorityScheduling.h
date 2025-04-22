#ifndef PREEMPTIVEPRIORITYSCHEDULING_H
#define PREEMPTIVEPRIORITYSCHEDULING_H

#include "Scheduler.h"
#include "Process.h"
#include <vector>

class PreemptivePriorityScheduler : public Scheduler
{
    Q_OBJECT

public:
    explicit PreemptivePriorityScheduler(QObject *parent = nullptr, std::vector<Process> Processes = {});

    void schedule();
    void processNext();

private:
    int currentTime;
    void sortProcessesByPriority();
    void checkForArrival();
    void updateWaitingTimes();

signals:
    void dataChanged(int processNumber);
    void ProcessFinished(int processNumber, int waitingTime, int turnaroundTime);
};

#endif // PREEMPTIVEPRIORITYSCHEDULING_H

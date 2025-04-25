#ifndef PREEMPTIVEPRIORITYSCHEDULING_H
#define PREEMPTIVEPRIORITYSCHEDULING_H

#include "Scheduler.h"
#include <deque>

class PreemptivePriorityScheduler : public Scheduler
{
    Q_OBJECT

public:
    explicit PreemptivePriorityScheduler(QObject *parent = nullptr, std::vector<Process> Processes = {});
    ~PreemptivePriorityScheduler();

    void schedule() override;
    void addProcessPPS(Process* p);
    void addNewProcessPPS(Process* p);

private:
    static std::deque<Process*> mainqueue;
    static std::deque<Process*> ready;
    Process* current_process = nullptr;
    Process* running_process = nullptr;
    int timeQuantum = 0;
    int current_time;
    int totalWaitingTime;
    int totalTurnaroundTime;
    int completedProcesses;

    void checkArrival();
    void updateWaitingTimes();
    void preemptivePriorityScheduling(int Q);
    // في PreemptivePriorityScheduler.h
signals:
    void dataChanged(int processID);
    void ProcessFinished(int processID, int waitingTime, int turnaroundTime);

};

#endif // PREEMPTIVEPRIORITYSCHEDULING_H

#pragma once

#include "scheduler.h"
#include "process.h"
#include <QTimer>
#include <deque>
#include <vector>

class PreemptivePriorityScheduler : public Scheduler
{
    Q_OBJECT

public:
    /// ctor must take std::vector<Process> to match Scheduler(parent, Processes)
    PreemptivePriorityScheduler(QObject *parent, const std::vector<Process>& Processes);
    ~PreemptivePriorityScheduler() override;

    explicit PreemptivePriorityScheduler(QObject* parent = nullptr,
                                         const std::vector<Process>& Processes = {});


    /// two static queues just like RoundRobin
    static std::deque<Process*> mainqueue;
    static std::deque<Process*> readyQueue;

public slots:
    /// override of Scheduler::schedule()
    void schedule() override;

    /// slot to add a new process at runtime
    void addNewProcess(Process* p);

signals:
    /// emitted each time a time‐unit executes (or -1 for idle)
    void dataChanged(int processID);
    /// emitted when a process finishes
    void ProcessFinished(int processID, int waitingTime, int turnaroundTime);
    /// emitted once at end with averages
    void averagesCalculated(double avgTurnaround, double avgWaiting);

private:
    /// move arrived processes into readyQueue
    void checkArrival();
    /// the core preemptive-priority loop
    void runPriority();

    int current_time = 0;
    bool finished = false;
};

#ifndef SJFNONPREEMPTIVE_H
#define SJFNONPREEMPTIVE_H

#include "scheduler.h"
#include "process.h"
#include <vector>
#include <algorithm>
#include <queue>
#include <QTimer>

class SJFNonPreemptive : public Scheduler
{
public:
    explicit SJFNonPreemptive(QObject *parent, std::vector<Process> processes);
    ~SJFNonPreemptive();

    void schedule() override;
    void addNewProcess(Process* p) override;

private:
    std::vector<Process> processes;
    using Comparator = std::function<bool(Process*, Process*)>;
    std::priority_queue<Process*, std::vector<Process*>, Comparator> arrivedQueue;
    int totalWaitingTime;
    int totalTurnaroundTime;
    QTimer* schedulerTimer;
    int currentTime;
    int indexArrived;
    Process* currentProcess;
    int remainingExecTimeForProcess;


};

#endif // SJFNONPREEMPTIVE_H

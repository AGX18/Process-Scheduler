#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include "scheduler.h"
#include "process.h"
#include <vector>
#include <algorithm>
#include <deque>
#include <QTimer>

class RoundRobin : public Scheduler
{
private:
    std::vector<Process> processes;
    std::deque<Process*> arrivedQueue;
    int timeQ;
    int totalWaitingTime;
    int totalTurnaroundTime;
    QTimer* schedulerTimer;
    int currentTime;
    int indexArrived;
    Process* currentProcess;
    int remainingExecTimeForProcess;

public:
    explicit RoundRobin(QObject *parent, std::vector<Process> processes, int timeQ);
    ~RoundRobin();
    void schedule() override;
    void addNewProcess(Process* p) override;
};

#endif // ROUNDROBIN_H

#ifndef FCFS_H
#define FCFS_H

#include "scheduler.h"
#include "process.h"
#include <vector>
#include <algorithm>
#include <queue>
#include <QTimer>

class FCFS : public Scheduler
{
public:
    explicit FCFS(QObject *parent, std::vector<Process> processes);
    ~FCFS();
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

#endif // FCFS_H




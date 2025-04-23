#ifndef SRTF_SCHEDULER_H
#define SRTF_SCHEDULER_H

#include "scheduler.h"
#include "process.h"
#include <vector>
#include <algorithm>
#include <climits>
#include <deque>
#include <QTimer>

class SRTFScheduler : public Scheduler
{
private:
    std::vector<Process> processes;
    std::deque<Process*> arrivedQueue;
    int totalWaitingTime;
    int totalTurnaroundTime;
    QTimer* schedulerTimer;
    int currentTime;
    int indexArrived;
    Process* currentProcess;

public:
    explicit SRTFScheduler(QObject *parent, std::vector<Process> processes);
    ~SRTFScheduler();
    void schedule() override;
    void addNewProcess(Process* p) override;

private:
    void printResults();
};

#endif // SRTF_SCHEDULER_H

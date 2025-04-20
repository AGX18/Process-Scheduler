#ifndef SJF_SCHEDULER_H
#define SJF_SCHEDULER_H

#include "scheduler.h"
#include "process.h"
#include <vector>
#include <algorithm>
#include <deque>
#include <QTimer>
#include <mutex>
#include <climits>
#include <QDebug>

/**
 * @brief The SJFScheduler class
 * Shortest Job First (non-preemptive) scheduling algorithm
 */
class SJFScheduler : public Scheduler
{


public:
    /**
     * @brief SJFScheduler
     * @param parent QObject parent
     * @param processes Pointer to vector of processes to schedule
     */
    explicit SJFScheduler(QObject* parent , std::vector<Process> processes );

    /**
     * @brief schedule
     * Implements the SJF scheduling algorithm
     */
    ~SJFScheduler();
    void schedule() override;
    void addNewProcess(Process* p) override;
    void stopScheduler();

private:
    /**
     * @brief printResults
     * Prints average waiting and turnaround times
     */

    std::vector<Process> processes;
    std::deque<Process*> arrivedQueue;
    QTimer* schedulerTimer;
    int currentTime;
    int indexArrived;
    Process* currentProcess;
    std::mutex queueMutex;
    bool running;
    void schedulerTick();
    Process* getShortestJob();
};

#endif // SJF_SCHEDULER_H

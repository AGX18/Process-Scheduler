#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include "scheduler.h"
#include "process.h"
#include <vector>
#include <algorithm>
#include <deque>
#include <QDebug>
#include <QTimer>
#include <mutex>
#include <climits>


/**
 * @brief The PriorityScheduler class
 * Priority (non-preemptive) scheduling algorithm
 */
class PriorityScheduler : public Scheduler
{

public:
    /**
     * @brief PriorityScheduler
     * @param parent QObject parent
     * @param processes Pointer to vector of processes to schedule
     */
    explicit PriorityScheduler(QObject* parent , std::vector<Process> processes );

    /**
     * @brief schedule
     * Implements the Priority scheduling algorithm
     */
    ~PriorityScheduler();
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
    Process* getHighestPriorityProcess();

};

#endif // PRIORITY_SCHEDULER_H

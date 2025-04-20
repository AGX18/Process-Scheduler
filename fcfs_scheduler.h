#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "scheduler.h"
#include "process.h"
#include <algorithm>
#include <queue>
#include <mutex>
#include <deque>
#include <QTimer>

/**
 * @brief The FCFSScheduler class
 * First-Come First-Served scheduling algorithm
 */
class FCFSScheduler : public Scheduler
{

public:
    /**
     * @brief FCFSScheduler
     * @param parent QObject parent
     * @param processes Pointer to vector of processes to schedule
     */
    explicit FCFSScheduler(QObject* parent , std::vector<Process> processes);

    /**
     * @brief schedule
     * Implements the FCFS scheduling algorithm
     *
     */

    ~FCFSScheduler();
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

};


#endif // FCFS_SCHEDULER_H

#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "scheduler.h"
#include <algorithm>

/**
 * @brief The FCFSScheduler class
 * First-Come First-Served scheduling algorithm
 */
class FCFSScheduler : public Scheduler
{
    Q_OBJECT

public:
    /**
     * @brief FCFSScheduler
     * @param parent QObject parent
     * @param processes Pointer to vector of processes to schedule
     */
    explicit FCFSScheduler(QObject* parent = nullptr, std::vector<Process>* processes = nullptr);

    /**
     * @brief schedule
     * Implements the FCFS scheduling algorithm
     */
    void schedule() override;

private:
    /**
     * @brief printResults
     * Prints average waiting and turnaround times
     */
    void printResults();
};

#endif // FCFS_SCHEDULER_H
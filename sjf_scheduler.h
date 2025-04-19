#ifndef SJF_SCHEDULER_H
#define SJF_SCHEDULER_H

#include "scheduler.h"
#include <algorithm>
#include <climits>

/**
 * @brief The SJFScheduler class
 * Shortest Job First (non-preemptive) scheduling algorithm
 */
class SJFScheduler : public Scheduler
{
    Q_OBJECT

public:
    /**
     * @brief SJFScheduler
     * @param parent QObject parent
     * @param processes Pointer to vector of processes to schedule
     */
    explicit SJFScheduler(QObject* parent = nullptr, std::vector<Process>* processes = nullptr);

    /**
     * @brief schedule
     * Implements the SJF scheduling algorithm
     */
    void schedule() override;

private:
    /**
     * @brief printResults
     * Prints average waiting and turnaround times
     */
    void printResults();
};

#endif // SJF_SCHEDULER_H

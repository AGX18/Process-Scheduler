#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include "scheduler.h"
#include <algorithm>
#include <climits>

/**
 * @brief The PriorityScheduler class
 * Priority (non-preemptive) scheduling algorithm
 */
class PriorityScheduler : public Scheduler
{
    Q_OBJECT

public:
    /**
     * @brief PriorityScheduler
     * @param parent QObject parent
     * @param processes Pointer to vector of processes to schedule
     */
    explicit PriorityScheduler(QObject* parent = nullptr, std::vector<Process>* processes = nullptr);

    /**
     * @brief schedule
     * Implements the Priority scheduling algorithm
     */
    void schedule() override;

private:
    /**
     * @brief printResults
     * Prints average waiting and turnaround times
     */
    void printResults();
};

#endif // PRIORITY_SCHEDULER_H
#ifndef SRTF_SCHEDULER_H
#define SRTF_SCHEDULER_H

#include "scheduler.h"
#include <algorithm>
#include <climits>

/**
 * @brief The SRTFScheduler class
 * Shortest Remaining Time First (preemptive SJF) scheduling algorithm
 */
class SRTFScheduler : public Scheduler
{
    Q_OBJECT

public:
    /**
     * @brief SRTFScheduler
     * @param parent QObject parent
     * @param processes Pointer to vector of processes to schedule
     */
    explicit SRTFScheduler(QObject* parent = nullptr, std::vector<Process>* processes = nullptr);

    /**
     * @brief schedule
     * Implements the SRTF scheduling algorithm
     */
    void schedule() override;

private:
    /**
     * @brief printResults
     * Prints average waiting and turnaround times
     */
    void printResults();
};

#endif // SRTF_SCHEDULER_H

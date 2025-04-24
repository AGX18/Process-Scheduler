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
#include <deque>

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
    void addNewProcessSJFNP(Process* p) ;
    static void addProcessSJFNP(Process* p);

private:
    /**
     * @brief printResults
     * Prints average waiting and turnaround times
     */

    int completed = 0;
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;
    int current_time = 0;

    static std::deque<Process*> sjf_mainqueue;
    static std::deque<Process*> sjf_ready;
    void checkArrival();
    void RUNSJFNP();
};

#endif // SJF_SCHEDULER_H

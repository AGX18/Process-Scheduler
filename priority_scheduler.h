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
    void addNewProcessPNP(Process* p) ;
    static void addProcessPNP(Process* p);

private:
    /**
     * @brief printResults
     * Prints average waiting and turnaround times
     */

    int completed = 0;
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;
    int current_time = 0;


    static std::deque<Process*> pnp_mainqueue;
    static std::deque<Process*> pnp_ready;
    void checkArrival();
    void RUNPNP();

};

#endif // PRIORITY_SCHEDULER_H

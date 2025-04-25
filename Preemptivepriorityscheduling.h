#ifndef PREEMPTIVEPRIORITYSCHEDULER_H
#define PREEMPTIVEPRIORITYSCHEDULER_H

#include "scheduler.h"
#include "process.h"
#include <vector>
#include <deque>
#include <QObject>
#include <algorithm>
#include <QDebug>

/**
 * @brief The PreemptivePriorityScheduler class
 * Preemptive Priority scheduling algorithm
 */
class PreemptivePriorityScheduler : public Scheduler
{
public:
    /**
     * @brief PreemptivePriorityScheduler
     * @param parent QObject parent
     * @param processes Vector of processes to schedule
     */
    explicit PreemptivePriorityScheduler(QObject* parent, std::vector<Process> processes);

    /**
     * @brief ~PreemptivePriorityScheduler
     * Destructor
     */
    ~PreemptivePriorityScheduler();

    /**
     * @brief schedule
     * Starts the scheduling process (runs in a separate thread)
     */
    void schedule() override;

    /**
     * @brief addNewProcessPPS
     * Adds a new process dynamically during runtime
     * @param p Pointer to the new process
     */
    void addNewProcessPPS(Process* p);

    /**
     * @brief addProcessPPS
     * Adds process to the main queue
     * @param p Pointer to the process
     */
    static void addProcessPPS(Process* p);

private:
    static std::deque<Process*> mainqueue;
    static std::deque<Process*> ready;

    int current_time = 0;
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;
    int completedProcesses = 0;

    void checkArrival();
    void updateWaitingTimes();

    void RUNP();  // main scheduling function used in thread
};

#endif // PREEMPTIVEPRIORITYSCHEDULER_H

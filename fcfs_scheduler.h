#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "scheduler.h"
#include "process.h"
#include <algorithm>
#include <queue>
#include <deque>
#include <QTimer>
#include <memory>

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
    explicit FCFSScheduler(QObject* parent, std::vector<Process> processes);
    ~FCFSScheduler();

    /**
     * @brief schedule
     * Implements the FCFS scheduling algorithm
     *
     */


    void schedule() override;
    static void addProcessFCFS(Process* p);
    void addNewProcessFCFS(Process* p);


private:
    /**
     * @brief printResults
     * Prints average waiting and turnaround times
     */
    int current_time = 0;
    int completed = 0;
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;

  static std::deque<Process*> fcfs_mainqueue;
  static std::deque<Process*> fcfs_ready;

    void checkArrival();
    void RUNFCFS();
};



#endif // FCFS_SCHEDULER_H

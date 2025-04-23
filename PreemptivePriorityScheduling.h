#pragma once
#include "scheduler.h"
#include "mainwindow.h"
#include "process.h"
#include <QTimer>
#include <deque>
#include <vector>


class PreemptivePriorityScheduler : public Scheduler
{
    Q_OBJECT
public:
    // Constructor
    PreemptivePriorityScheduler(QObject *parent, std::vector<Process> Processes);

    // Destructor
    ~PreemptivePriorityScheduler();

    // Public members (queues)
    static std::deque<Process*> ready;
    static std::deque<Process*> mainqueue;

public slots:
    // Method to start scheduling
    void schedule() override;

    // Method to add a new process to mainqueue
    static void addProcessPPS(Process* p);

    // Method to add a new process and sort by arrival time
    void addNewProcessPPS(Process* p);

private:
    // Core scheduling function
    void preemptivePriorityScheduling(int Q);

    // Method to check if any process has arrived
    void checkArrival();

    // Helper method to sort processes by priority
    void sortProcessesByPriority();

    // Method to update waiting times of processes
    void updateWaitingTimes();

    // Private members
    int current_time = 0;
    int timeQuantum;
    int completedProcesses = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    Process* current_process = nullptr;
    QVector<bool> added;
    bool finished = false;


};


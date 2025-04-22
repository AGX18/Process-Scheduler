#ifndef PREEMPTIVEPRIORITYSCHEDULING_H
#define PREEMPTIVEPRIORITYSCHEDULING_H

#include "process.h"
#include <vector>

class MainWindow;

class PreemptivePriorityScheduling {
public:
    PreemptivePriorityScheduling(int n);
    ~PreemptivePriorityScheduling();

    void setProcesses(const std::vector<Process*>& new_processes);
    void run();
    Process* getHighestPriorityProcess();

    // إشارة يتم إطلاقها عند تحديث حالة عملية قيد التشغيل


    void setScheduler(MainWindow* schedulerWindow) { this->scheduler = schedulerWindow; }

private:
    int n;
    int completed;
    int time;
    float total_waiting_time;
    float total_turnaround_time;
    std::vector<Process*> processes;
    MainWindow* scheduler = nullptr;

signals:
    void dataChanged(int processId);
    void ProcessFinished(int processId, int waitingTime, int turnaroundTime);
};

#endif // PREEMPTIVEPRIORITYSCHEDULING_H

// PreemptivePriorityScheduling.h

#ifndef PREEMPTIVEPRIORITYSCHEDULING_H
#define PREEMPTIVEPRIORITYSCHEDULING_H

#include <QObject>
#include <deque>
#include <vector>
#include "process.h"
#include "scheduler.h"

class PreemptivePriorityScheduler : public Scheduler {
    Q_OBJECT

public:
    explicit PreemptivePriorityScheduler(QObject *parent, const std::vector<Process>& processes);
    ~PreemptivePriorityScheduler() override;


    void schedule() override;

    void addProcessPPS(Process* p);
    void addNewProcessPPS(Process* p);

signals:
    void currentProcessChanged(Process* current);  // للإعلام بالعملية الجارية

private:
    void preemptivePriorityScheduling(int Q);
    void checkArrival();

    static std::deque<Process*> mainqueue;
    static std::deque<Process*> ready;

    int current_time;
    int completedProcesses;
    int totalWaitingTime;
    int totalTurnaroundTime;
    int timeQuantum;
    int totalProcesses;          // عدد العمليات الثابت
    std::vector<bool> added;     // لتجنّب إضافة العملية أكثر من مرة
};

#endif // PREEMPTIVEPRIORITYSCHEDULING_H

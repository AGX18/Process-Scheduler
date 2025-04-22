#ifndef PREEMPTIVEPRIORITYSCHEDULING_H
#define PREEMPTIVEPRIORITYSCHEDULING_H

#include "scheduler.h"
#include "process.h"
#include <vector>
#include <deque>
#include <QObject>

class PreemptivePriorityScheduling : public Scheduler
{
    Q_OBJECT

public:
    PreemptivePriorityScheduling(QObject *parent, std::vector<Process> Processes);
    ~PreemptivePriorityScheduling();

    static std::deque<Process*> mainqueue;
    static std::deque<Process*> ready;

    void schedule() override;
    void addProcess(Process* p);
    void addNewProcess(Process* p);

private:
    int current_time=0;
    void checkArrival();
    void PriorityPreemptive();
};

#endif // PREEMPTIVEPRIORITYSCHEDULING_H

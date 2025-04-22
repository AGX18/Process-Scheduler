#ifndef PREEMPTIVEPRIORITYSCHEDULING_H
#define PREEMPTIVEPRIORITYSCHEDULING_H

#include "Scheduler.h"
#include "Process.h"
#include <vector>

class PreemptivePriorityScheduler : public Scheduler
{
    Q_OBJECT

public:
    PreemptivePriorityScheduler(QObject *parent, std::vector<Process> Processes);
    ~PreemptivePriorityScheduler() = default;

    void schedule();  // جدولة العمليات
    void sortProcessesByPriority();  // ترتيب العمليات حسب الأولوية
    void checkForArrival();  // التحقق من وصول العمليات الجديدة
    void updateWaitingTimes();  // تحديث أوقات الانتظار

private:
    int currentTime;
    int totalWaitingTime;
    int totalTurnaroundTime;
    int completedProcesses;
};

#endif // PREEMPTIVEPRIORITYSCHEDULING_H

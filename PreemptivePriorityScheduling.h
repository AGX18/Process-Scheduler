
#ifndef PREEMPTIVEPRIORITYSCHEDULER_H
#define PREEMPTIVEPRIORITYSCHEDULER_H

#include "scheduler.h"

class PreemptivePriorityScheduler : public Scheduler
{
    Q_OBJECT

public:
    explicit PreemptivePriorityScheduler(QObject *parent = nullptr, std::vector<Process> Processes = {});
    void schedule() override;
    void processNext();

private:
    int currentTime; // لحساب الوقت الحالي

    void sortProcessesByPriority(); // ترتيب العمليات حسب الأولوية
    void checkForArrival(Process* process); // التحقق إذا وصلت عملية جديدة
    void updateWaitingTimes(); // تحديث أوقات الانتظار
};

#endif // PREEMPTIVEPRIORITYSCHEDULER_H

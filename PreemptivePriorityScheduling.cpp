#include "PreemptivePriorityScheduling.h"
#include <algorithm>

PreemptivePriorityScheduler::PreemptivePriorityScheduler(QObject *parent, std::vector<Process> Processes)
    : Scheduler{parent, std::move(Processes)}, currentTime(0)
{
    qInfo() << this << "constructed Preemptive Priority Scheduler";
}

void PreemptivePriorityScheduler::schedule()
{
    while (!Processes.empty()) {
        // ترتيب العمليات حسب الأولوية (الأعلى أولوية أولاً)
        sortProcessesByPriority();

        Process* processToRun = nullptr;

        // محاولة العثور على العملية التي يجب تشغيلها الآن
        for (auto& process : Processes) {
            if (process.getArrivalTime() <= currentTime && process.getRemainingTime() > 0) {
                if (processToRun == nullptr || process.getPriority() > processToRun->getPriority()) {
                    processToRun = &process;
                }
            }
        }

        if (processToRun != nullptr) {
            // قم بتشغيل العملية
            processToRun->decrementRemainingTime();

            if (processToRun->getRemainingTime() == 0) {
                processToRun->setFinishTime(currentTime);
                emit dataChanged(processToRun->getProcessNumber());
                emit ProcessFinished(processToRun->getProcessNumber(), processToRun->getWaitingTime(), processToRun->getTurnaroundTime());
            }

            currentTime++;

            // استدعاء waitOneSecond لتأخير التنفيذ لمدة ثانية واحدة قبل الانتقال إلى الخطوة التالية
            waitOneSecond();

            checkForArrival(processToRun); // تحقق من أي عمليات جديدة قد وصلت
            updateWaitingTimes();
        }
    }
}

void PreemptivePriorityScheduler::processNext()
{
    // فائدة هذه الوظيفة يمكن أن تكون في تنفيذ عملية معينة أو التحكم في الانتقالات
    qInfo() << "Processing next step at time" << currentTime;
}

void PreemptivePriorityScheduler::sortProcessesByPriority()
{
    std::sort(Processes.begin(), Processes.end(), [](const Process& a, const Process& b) {
        return a.getPriority() > b.getPriority(); // ترتيب العمليات حسب الأولوية من الأكبر إلى الأصغر
    });
}

void PreemptivePriorityScheduler::checkForArrival(Process* process)
{
    // تحقق من أن أي عملية وصلت في الوقت الحالي
    for (auto& p : Processes) {
        if (p.getArrivalTime() <= currentTime && p.getRemainingTime() > 0) {
            // تحديث عملية وصلت
            p.setWaitingTime(currentTime - p.getArrivalTime());
            p.setStartTime(currentTime);
            emit dataChanged(p.getProcessNumber());
        }
    }
}

void PreemptivePriorityScheduler::updateWaitingTimes()
{
    for (auto& process : Processes) {
        if (process.getRemainingTime() > 0 && process.getArrivalTime() <= currentTime) {
            process.setWaitingTime(process.getWaitingTime() + 1);
        }
    }
}

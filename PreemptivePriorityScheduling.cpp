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
        sortProcessesByPriority();

        Process* processToRun = nullptr;

        for (auto& process : Processes) {
            if (process.getArrivalTime() <= currentTime && process.getRemainingTime() > 0) {
                if (processToRun == nullptr || process.getPriority() < processToRun->getPriority()) {
                    processToRun = &process;
                }
            }
        }

        if (processToRun != nullptr) {
            // تشغيل العملية
            processToRun->decrementRemainingTime();

            if (processToRun->getRemainingTime() == 0) {
                processToRun->setFinishTime(currentTime + 1);  // تحديث وقت الانتهاء
                emit dataChanged(processToRun->getProcessNumber());
                emit ProcessFinished(processToRun->getProcessNumber(), processToRun->getWaitingTime(), processToRun->getTurnaroundTime());
                // لا نقوم بحذف العملية الآن
            } else {
                emit dataChanged(processToRun->getProcessNumber());
            }

            currentTime++;

            waitOneSecond();

            checkForArrival();
            updateWaitingTimes();
        }
        else {
            currentTime++;
            waitOneSecond();
        }
    }
}


void PreemptivePriorityScheduler::processNext()
{
    qInfo() << "Processing next step at time" << currentTime;
}

void PreemptivePriorityScheduler::sortProcessesByPriority()
{
    std::sort(Processes.begin(), Processes.end(), [](const Process& a, const Process& b) {
        return a.getPriority() < b.getPriority(); // الأقل رقم أعلى أولوية
    });
}

void PreemptivePriorityScheduler::checkForArrival()
{
    for (auto& p : Processes) {
        if (p.getArrivalTime() <= currentTime && p.getRemainingTime() > 0) {
            if (p.getStartTime() == -1) {
                p.setStartTime(currentTime);
            }
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

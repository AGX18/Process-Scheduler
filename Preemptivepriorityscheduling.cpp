

#include "PreemptivePriorityScheduling.h"
#include <algorithm>
#include <QDebug>
#include <QThread>
#include "process.h"

std::deque<Process*> PreemptivePriorityScheduler::mainqueue;
std::deque<Process*> PreemptivePriorityScheduler::ready;

PreemptivePriorityScheduler::PreemptivePriorityScheduler(QObject *parent, const std::vector<Process>& processes)
    : Scheduler(parent, processes),
    current_time(0),
    completedProcesses(0),
    totalWaitingTime(0),
    totalTurnaroundTime(0)
{
    // نحفظ عدد العمليات عند بداية التنفيذ
    totalProcesses = Processes.size();
    added.resize(totalProcesses, false);

    // نفرز mainqueue حسب Arrival Time (رغم أنه فارغ الآن)
    std::sort(mainqueue.begin(), mainqueue.end(),
              [](Process* a, Process* b){ return a->getArrivalTime() < b->getArrivalTime(); });
}

PreemptivePriorityScheduler::~PreemptivePriorityScheduler() {
    qDebug() << "PreemptivePriorityScheduler destroyed";
}

void PreemptivePriorityScheduler::addProcessPPS(Process* p) {
    mainqueue.push_back(p);
}

void PreemptivePriorityScheduler::addNewProcessPPS(Process* p) {
    qDebug() << "New process added: P" << p->getProcessNumber();
    mainqueue.push_back(p);

}

void PreemptivePriorityScheduler::schedule() {
    qDebug() << "بدء جدولة الأولوية الاستباقية...";
    // نفّذ الخوارزمية في نفس خيط Qt بعد moveToThread
    preemptivePriorityScheduling(timeQuantum);
}

void PreemptivePriorityScheduler::checkArrival() {
    // نضيف أي عملية وصلت زمنها إلى قائمة ready
    while (!mainqueue.empty()) {
        Process* P = mainqueue.front();
        if (P->getArrivalTime() <= current_time) {
            ready.push_back(P);
            mainqueue.pop_front();
        } else {
            break;
        }
    }
}
void PreemptivePriorityScheduler::preemptivePriorityScheduling(int Q) {
    int valid = 0;

    // شرط الخروج بعد إنهاء العدد الثابت من العمليات
    while (completedProcesses < totalProcesses) {
        checkArrival();

        if (ready.empty()) {
            // فترة idle قصيرة
            current_time++;
            waitOneSecond();
            continue;
        }

        // نفرز قائمة الجاهزة حسب الأولوية (القيمة الأقل أولاً)
        std::sort(ready.begin(), ready.end(),
                  [](Process* a, Process* b){ return a->getPriority() < b->getPriority(); });

        Process* current_process = ready.front();
        if (current_process == nullptr) continue; // تحقق من المؤشر قبل استخدامه

        // تدوير بعد انتهاء الـ timeQuantum
        if (valid == Q && current_process->getRemainingTime() > 0) {
            ready.pop_front();
            ready.push_back(current_process);
            current_process = ready.front();
            valid = 0;
        }

        if (valid < Q) {
            // حدّث المؤشّر الثابت للعملية الجارية
            Scheduler::running_process = current_process;
            // أرسل الإشارات للواجهة
            emit currentProcessChanged(current_process);
            emit dataChanged(current_process->getProcessNumber());

            // محاكاة مرور ثانية
            waitOneSecond();

            // نفّذ وحدة زمن على العملية
            current_process->decrementRemainingTime();
            valid++;
            current_time++;

            // إذا انتهت العملية
            if (current_process->getRemainingTime() == 0) {
                ready.pop_front();
                int tat = current_time - current_process->getArrivalTime();
                int wt  = tat - current_process->getBurstTime();
                current_process->setTurnaroundTime(tat);
                current_process->setWaitingTime(wt);

                emit ProcessFinished(current_process->getProcessNumber(), wt, tat);

                totalTurnaroundTime += tat;
                totalWaitingTime     += wt;
                valid = 0;
                completedProcesses++;
            }
        }
    }

    // عرض المعدلات في الـ debug
    qDebug() << "Avg Waiting =" << (double)totalWaitingTime/totalProcesses
             << "Avg Turnaround =" << (double)totalTurnaroundTime/totalProcesses;
}

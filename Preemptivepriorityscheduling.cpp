#include "PreemptivePriorityScheduling.h"
#include "mainwindow.h"
#include <algorithm>
#include <QDebug>
#include <thread>

PreemptivePriorityScheduler::PreemptivePriorityScheduler(QObject *parent, std::vector<Process> Processes)
    : Scheduler(parent, std::move(Processes)), current_time(0), totalWaitingTime(0), totalTurnaroundTime(0), completedProcesses(0)
{
    qInfo() << this << "constructed Preemptive Priority Scheduler";

    for (Process& p : this->Processes) {
        mainqueue.push_back(&p);
    }

    std::sort(mainqueue.begin(), mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });
}

std::deque<Process*> PreemptivePriorityScheduler::mainqueue;
std::deque<Process*> PreemptivePriorityScheduler::ready;

PreemptivePriorityScheduler::~PreemptivePriorityScheduler() {
    qDebug() << "PreemptivePriorityScheduler destructor";
}

void PreemptivePriorityScheduler::checkArrival() {
    while (!mainqueue.empty()) {
        Process* P = mainqueue.front();
        if (P->getArrivalTime() <= current_time) {
            if (std::find(ready.begin(), ready.end(), P) == ready.end()) {
                ready.push_back(P);
                qDebug() << "Process P" << P->getProcessNumber() << "added to ready queue at time" << current_time;
            }
            mainqueue.pop_front();
        } else {
            break;
        }
    }
}


void PreemptivePriorityScheduler::addProcessPPS(Process* p) {
    mainqueue.push_back(p);
    qDebug() << "Process P" << p->getProcessNumber() << "added to mainqueue";
}

void PreemptivePriorityScheduler::addNewProcessPPS(Process* p) {
    qDebug() << "New process added: " << p->getProcessNumber();
    mainqueue.push_back(p);

    std::sort(mainqueue.begin(), mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });

    Processes.push_back(*p);
}

void PreemptivePriorityScheduler::updateWaitingTimes() {
    for (Process& process : Processes) {
        if (process.getRemainingTime() > 0 &&
            process.getArrivalTime() <= current_time &&
            process.getStartTime() != -1 &&
            &process != Scheduler::running_process) {
            process.setWaitingTime(process.getWaitingTime() + 1);
            qDebug() << "Updated waiting time for Process P" << process.getProcessNumber() << ": " << process.getWaitingTime();
        }
    }
}

void PreemptivePriorityScheduler::schedule() {
    std::thread t(&PreemptivePriorityScheduler::preemptivePriorityScheduling, this, timeQuantum);
    t.detach();
}

void PreemptivePriorityScheduler::preemptivePriorityScheduling(int Q) {
    qDebug() << "Number of processes: " << mainqueue.size();

    int valid = 0;
    while (completedProcesses < Processes.size()) {
        checkArrival();

        if (ready.empty()) {
            qDebug() << "Ready queue is empty, waiting...";
            current_time++;
            QThread::msleep(1000);
            continue;
        }

        // ترتيب العمليات في الـ ready queue حسب الأولوية
        std::sort(ready.begin(), ready.end(), [](Process* a, Process* b) {
            return a->getPriority() < b->getPriority();
        });

        Scheduler::current_process = ready.front();

        // تحقق إذا كان فيه عملية جديدة بأولوية أعلى من العملية الجارية (Preemption)
        if (Scheduler::current_process != Scheduler::running_process && Scheduler::running_process != nullptr && Scheduler::current_process->getPriority() < Scheduler::running_process->getPriority()) {
            qDebug() << "Preempting process P" << Scheduler::running_process->getProcessNumber()
            << " for new process P" << Scheduler::current_process->getProcessNumber();
            ready.push_front(Scheduler::running_process);
            Scheduler::running_process = nullptr;
        }

        // إذا كانت العملية الحالية قيد التشغيل وكان الـ valid أقل من Q
        if (valid < Q) {
            Scheduler::running_process = Scheduler::current_process;

            if (Scheduler::current_process->getStartTime() == -1) {
                Scheduler::current_process->setStartTime(current_time);
                qDebug() << "Process P" << Scheduler::current_process->getProcessNumber() << "started at time" << current_time;
            }

            QThread::msleep(1000);  // Simulate the time unit

            Scheduler::current_process->decrementRemainingTime();
            int rem = Scheduler::current_process->getRemainingTime();
            emit dataChanged(Scheduler::current_process->getProcessNumber());

            qDebug() << "Remaining time for process P" << Scheduler::current_process->getProcessNumber() << ": " << rem;

            updateWaitingTimes();

            current_time++;
            valid++;

            if (rem == 0) {
                ready.pop_front();

                int turnaround_time = current_time - Scheduler::current_process->getArrivalTime();
                int waiting_time = turnaround_time - Scheduler::current_process->getBurstTime();

                Scheduler::current_process->setTurnaroundTime(turnaround_time);
                Scheduler::current_process->setWaitingTime(waiting_time);

                emit ProcessFinished(Scheduler::current_process->getProcessNumber(), waiting_time, turnaround_time);

                totalTurnaroundTime += turnaround_time;
                totalWaitingTime += waiting_time;
                completedProcesses++;

                qDebug() << "Process P" << Scheduler::current_process->getProcessNumber() << "finished. Turnaround Time:" << turnaround_time << ", Waiting Time:" << waiting_time;

                Scheduler::current_process = nullptr;
                Scheduler::running_process = nullptr;
                valid = 0;
            }
        }

        // تأكد من معالجة العمليات المتأخرة والانتظار بينها
    }

    qDebug() << "Average waiting time = " << totalWaitingTime / Processes.size();
    qDebug() << "Average turnaround time = " << totalTurnaroundTime / Processes.size();
}

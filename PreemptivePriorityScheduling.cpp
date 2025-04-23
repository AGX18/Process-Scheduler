#include "PreemptivePriorityScheduling.h"
#include "mainwindow.h"
#include <algorithm>
#include <QDebug>
#include <thread>

PreemptivePriorityScheduler::PreemptivePriorityScheduler(QObject *parent, std::vector<Process> Processes)
    : Scheduler(parent, std::move(Processes)), current_time(0), totalWaitingTime(0), totalTurnaroundTime(0), completedProcesses(0)
{
    added.resize(Processes.size(), false);
    qInfo() << this << "constructed Preemptive Priority Scheduler";

    // Sort processes by arrival time
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
            ready.push_back(P);
            mainqueue.pop_front();
        } else {
            break;
        }
    }
}

void PreemptivePriorityScheduler::addProcessPPS(Process* p) {
    mainqueue.push_back(p);
}

void PreemptivePriorityScheduler::addNewProcessPPS(Process* p) {
    qDebug() << "New process added: " << p->getProcessNumber();
    mainqueue.push_back(p);

    // Sort processes by arrival time
    std::sort(mainqueue.begin(), mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });

    Processes.push_back(*p);
}

void PreemptivePriorityScheduler::sortProcessesByPriority() {
    // Sort processes by priority (lower value means higher priority)
    std::sort(Processes.begin(), Processes.end(), [](const Process& a, const Process& b) {
        return a.getPriority() < b.getPriority();
    });
}

void PreemptivePriorityScheduler::updateWaitingTimes() {
    for (auto& process : Processes) {
        if (process.getRemainingTime() > 0 && process.getArrivalTime() <= current_time && process.getStartTime() != -1) {
            process.setWaitingTime(process.getWaitingTime() + 1);
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
            current_time++;
            QThread::msleep(1000);  // هنا استبدلنا waitOneSecond() بـ msleep
            continue;
        }

        // Sort ready queue based on process priority
        std::sort(ready.begin(), ready.end(), [](Process* a, Process* b) {
            return a->getPriority() < b->getPriority(); // Higher priority first
        });

        current_process = ready.front();

        if (valid == timeQuantum && current_process->getRemainingTime() > 0) {
            ready.pop_front();
            ready.push_back(current_process);
            current_process = ready.front();
            valid = 0;
        }

        if (valid < timeQuantum) {
            running_process = current_process;
            QThread::msleep(1000);  // هنا أيضا استبدلنا waitOneSecond() بـ msleep
            current_process->decrementRemainingTime();
            int rem = current_process->getRemainingTime();
            emit dataChanged(current_process->getProcessNumber());

            qDebug() << "Remaining: " << rem;
            valid++;

            current_time++;

            if (current_process->getRemainingTime() == 0) {
                ready.pop_front();
                int turnaround_time = current_time - current_process->getArrivalTime();
                int waiting_time = turnaround_time - current_process->getBurstTime();
                current_process->setTurnaroundTime(turnaround_time);
                current_process->setWaitingTime(waiting_time);
                emit ProcessFinished(current_process->getProcessNumber(), waiting_time, turnaround_time);

                current_process = nullptr;
                running_process = nullptr;

                totalTurnaroundTime += turnaround_time;
                totalWaitingTime += waiting_time;
                valid = 0;
                completedProcesses++;
            }
        }
    }

    qDebug() << "Average waiting time = " << totalWaitingTime / Processes.size();
    qDebug() << "Average turnaround time = " << totalTurnaroundTime / Processes.size();
}

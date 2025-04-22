#include "PreemptivePriorityScheduling.h"
#include "process.h"
#include "mainwindow.h"
#include <QDebug>
#include <iostream>
#include <vector>
#include <algorithm>

PreemptivePriorityScheduling::PreemptivePriorityScheduling(int n)
    : n(n), completed(0), time(0), total_waiting_time(0), total_turnaround_time(0) {
    processes.resize(n);
}

PreemptivePriorityScheduling::~PreemptivePriorityScheduling() {
    for (Process* p : processes) {
        delete p;
    }
    processes.clear();
}

// دالة لتعيين العمليات من واجهة المستخدم
void PreemptivePriorityScheduling::setProcesses(const std::vector<Process*>& new_processes) {
    processes = new_processes;
}

void PreemptivePriorityScheduling::run() {
    int currently_running = -1;

    while (completed < n) {
        Process* next_process = getHighestPriorityProcess();

        if (next_process != nullptr) {
            // لاقينا process
            next_process->decrementRemainingTime();
            time++;

            if (next_process->getRemainingTime() == 0) {
                completed++;
                next_process->setFinishTime(time);
                next_process->setTurnaroundTime(next_process->getFinishTime() - next_process->getArrivalTime());
                next_process->setWaitingTime(next_process->getTurnaroundTime() - next_process->getBurstTime());

                total_turnaround_time += next_process->getTurnaroundTime();
                total_waiting_time += next_process->getWaitingTime();
            }
        } else {
            // مفيش ولا process جاهز، نزود الوقت
            time++;
        }
    }

    // حساب المتوسطات
    if (n > 0) {
        qDebug() << "Average Turnaround Time = " << total_turnaround_time / n;
        qDebug() << "Average Waiting Time = " << total_waiting_time / n;
    }
}

Process* PreemptivePriorityScheduling::getHighestPriorityProcess() {
    Process* best = nullptr;
    int highest_priority = 1e9;

    for (Process* p : processes) {
        if (p->getArrivalTime() <= time && p->getRemainingTime() > 0) {
            if (p->getPriority() < highest_priority) {
                highest_priority = p->getPriority();
                best = p;
            }
        }
    }

    return best;
}

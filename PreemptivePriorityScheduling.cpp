#include "Preemptivepriorityscheduling.h"

std::deque<Process*> PreemptivePriorityScheduler::mainqueue;
std::deque<Process*> PreemptivePriorityScheduler::ready;

PreemptivePriorityScheduler::PreemptivePriorityScheduler(QObject* parent, std::vector<Process> Processes)
    : Scheduler(parent, Processes), current_time(0), totalTurnaroundTime(0), totalWaitingTime(0), completedProcesses(0)
{
    std::sort(mainqueue.begin(), mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });
}

PreemptivePriorityScheduler::~PreemptivePriorityScheduler()
{
    qDebug() << "Preemptive Priority destructor";
}

void PreemptivePriorityScheduler::addProcessPPS(Process* p)
{
    mainqueue.push_back(p);
}

void PreemptivePriorityScheduler::addNewProcessPPS(Process* p)
{
    qDebug() << "new process added : " << p->getProcessNumber();
    mainqueue.push_back(p);
    Processes.push_back(*p);
    std::sort(mainqueue.begin(), mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });
}

void PreemptivePriorityScheduler::checkArrival()
{
    while (!mainqueue.empty()) {
        Process* p = mainqueue.front();
        if (p->getArrivalTime() <= current_time) {
            ready.push_back(p);
            mainqueue.pop_front();
            std::stable_sort(ready.begin(), ready.end(), [](Process* a, Process* b) {
                if (a->getPriority() == b->getPriority())
                    return a->getArrivalTime() < b->getArrivalTime();
                return a->getPriority() < b->getPriority();
            });
        } else {
            break;
        }
    }
}

void PreemptivePriorityScheduler::schedule()
{
    std::thread t(&PreemptivePriorityScheduler::RUNP, this);
    t.detach();
}

void PreemptivePriorityScheduler::RUNP()
{
    Process* current_process = nullptr;

    while (completedProcesses < Processes.size()) {
        checkArrival();

        if (!ready.empty()) {
            Process* next_process = ready.front();
            // if current is null or next has higher priority (less number), preempt
            if (current_process == nullptr || next_process->getPriority() < current_process->getPriority()) {
                if (current_process != nullptr) {
                    ready.push_back(current_process);
                    std::stable_sort(ready.begin(), ready.end(), [](Process* a, Process* b) {
                        if (a->getPriority() == b->getPriority())
                            return a->getArrivalTime() < b->getArrivalTime();
                        return a->getPriority() < b->getPriority();
                    });
                }
                current_process = next_process;
                ready.pop_front();
            }
        }

        if (current_process == nullptr) {
            current_time++;
            waitOneSecond();
            continue;
        }

        running_process = current_process;
        waitOneSecond();
        current_process->decrementRemainingTime();
        emit dataChanged(current_process->getProcessNumber());
        qDebug() << "Executing P" << current_process->getProcessNumber()
                 << ", remaining time: " << current_process->getRemainingTime();

        current_time++;

        if (current_process->getRemainingTime() == 0) {
            int tat = current_time - current_process->getArrivalTime();
            int wt = tat - current_process->getBurstTime();

            current_process->setTurnaroundTime(tat);
            current_process->setWaitingTime(wt);

            emit ProcessFinished(current_process->getProcessNumber(), wt, tat);
            qDebug() << "Process P" << current_process->getProcessNumber()
                     << " finished. WT: " << wt << ", TAT: " << tat;

            totalTurnaroundTime += tat;
            totalWaitingTime += wt;
            completedProcesses++;
            current_process = nullptr;
            running_process = nullptr;
        }
    }

    qDebug() << "Average Waiting Time: " << totalWaitingTime / completedProcesses;
    qDebug() << "Average Turnaround Time: " << totalTurnaroundTime / completedProcesses;
}

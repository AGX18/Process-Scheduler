#include "sjf_scheduler.h"

std::deque<Process*> SJFScheduler::sjf_mainqueue;
std::deque<Process*> SJFScheduler::sjf_ready;

SJFScheduler::SJFScheduler(QObject *parent, std::vector<Process> Processes)
    : Scheduler(parent, Processes), current_time(0), avg_turnaround_time(0),
    avg_waiting_time(0), completed(0)
{
    // Sort by arrival time
    std::sort(sjf_mainqueue.begin(), sjf_mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });
}

SJFScheduler::~SJFScheduler()
{
    qDebug() << "SJF destructor";
}

void SJFScheduler::checkArrival()
{
    while (!sjf_mainqueue.empty()) {
        Process* P = sjf_mainqueue.front();
        if (P->getArrivalTime() <= current_time) {
            sjf_ready.push_back(P);
            sjf_mainqueue.pop_front();
            std::stable_sort(sjf_ready.begin(), sjf_ready.end(), [](Process* a, Process* b) {
                if (a->getBurstTime() == b->getBurstTime())
                    return a->getArrivalTime() < b->getArrivalTime();
                return a->getBurstTime() < b->getBurstTime();

            });
        } else {
            break;
        }
    }
    }


void SJFScheduler::schedule()
{
    std::thread t(&SJFScheduler::RUNSJFNP, this);
    t.detach();
}

void SJFScheduler::addProcessSJFNP(Process* p)
{
    sjf_mainqueue.push_back(p);

}

void SJFScheduler::addNewProcessSJFNP(Process* p)
{
    qDebug() << "new process added : " << p->getProcessNumber();
    Processes.push_back(*p);

    std::sort(sjf_mainqueue.begin(), sjf_mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });

}

void SJFScheduler::RUNSJFNP()
{
    qDebug() << "number of processes : " << sjf_mainqueue.size();

    Process* current_process=nullptr;

    while (completed < Processes.size()) {
        checkArrival();

        if (sjf_ready.empty() && current_process==nullptr) {
            current_time++;
            waitOneSecond();
            continue;
        }


        if (current_process==nullptr){
            current_process= sjf_ready.front();
            sjf_ready.pop_front();

        }

        if (current_process->getRemainingTime()==0){
            if(!sjf_ready.empty()){
                current_process= sjf_ready.front();
                sjf_ready.pop_front();
            }
            else {
                current_process =nullptr;
                running_process = current_process;
                continue;}
        }
        running_process = current_process;

        waitOneSecond();


        if (current_process->getRemainingTime() > 0) {  // Only decrement if > 0
            current_process->decrementRemainingTime();

            emit dataChanged(current_process->getProcessNumber());
            qDebug() << "Executing P" << current_process->getProcessNumber()
                     << ", remaining time: " << current_process->getRemainingTime();


            current_time++;

            if (current_process->getRemainingTime() == 0) {
                int turnaround_time = current_time - current_process->getArrivalTime();
                int waiting_time = turnaround_time - current_process->getBurstTime();

                current_process->setTurnaroundTime(turnaround_time);
                current_process->setWaitingTime(waiting_time);
                current_process->setRemainingTime(0); // Just in case

                emit ProcessFinished(current_process->getProcessNumber(), waiting_time, turnaround_time);
                qDebug() << "Process P" << current_process->getProcessNumber()
                         << " finished. WT: " << waiting_time << ", TAT: " << turnaround_time;

                avg_turnaround_time += turnaround_time;
                avg_waiting_time += waiting_time;
                completed++;


            }
        }
    }
    qDebug() << "Average waiting time = " << avg_waiting_time / completed;
    qDebug() << "Average turnaround time = " << avg_turnaround_time / completed;

    current_process = nullptr;
    running_process = nullptr;

}

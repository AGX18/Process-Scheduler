#include "priority_scheduler.h"
std::deque<Process*> PriorityScheduler::pnp_mainqueue;
std::deque<Process*> PriorityScheduler::pnp_ready;

PriorityScheduler::PriorityScheduler(QObject *parent, std::vector<Process> Processes)
    : Scheduler(parent, Processes), current_time(0), avg_turnaround_time(0),
    avg_waiting_time(0), completed(0)
{
    std::sort(pnp_mainqueue.begin(), pnp_mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });
}

PriorityScheduler::~PriorityScheduler()
{
    qDebug() << "Priority destructor";
}

void PriorityScheduler::checkArrival()
{
    while (!pnp_mainqueue.empty()) {
        Process* P = pnp_mainqueue.front();
        if (P->getArrivalTime() <= current_time) {
            pnp_ready.push_back(P);
            pnp_mainqueue.pop_front();
            std::stable_sort(pnp_ready.begin(), pnp_ready.end(), [](Process* a, Process* b) {
                if (a->getPriority() == b->getPriority())
                    return a->getArrivalTime() < b->getArrivalTime();
                 return a->getPriority() < b->getPriority();

            });
        } else {
            break;
        }
    }
}

void PriorityScheduler::schedule()
{
    std::thread t(&PriorityScheduler::RUNPNP, this);
    t.detach();
}

void PriorityScheduler::addProcessPNP(Process* p)
{
    pnp_mainqueue.push_back(p);
}

void PriorityScheduler::addNewProcessPNP(Process* p)
{
    qDebug() << "new process added : " << p->getProcessNumber();
    Processes.push_back(*p);
    std::sort(pnp_mainqueue.begin(), pnp_mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });

}

void PriorityScheduler::RUNPNP()
{
    qDebug() << "number of processes : " << pnp_mainqueue.size();

    Process* current_process=nullptr;

    while (completed < Processes.size()) {
        checkArrival();

        if (pnp_ready.empty() && current_process==nullptr) {
            current_time++;
            waitOneSecond();
            continue;
        }


        if (current_process==nullptr){
            current_process= pnp_ready.front();
            pnp_ready.pop_front();

        }

        if (current_process->getRemainingTime()==0){
            if(!pnp_ready.empty()){
            current_process= pnp_ready.front();
            pnp_ready.pop_front();
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

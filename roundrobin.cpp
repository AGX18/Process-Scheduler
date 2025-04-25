#include "roundrobin.h"
#include <QDebug>
#include <thread>
#include "mainwindow.h"

RoundRobin::RoundRobin(QObject *parent, std::vector<Process> Processes, int quantum)
    : Scheduler(parent, Processes) {
    added.resize(Processes.size(), false);
    this->timeQuantum = quantum;

    std::sort(mainqueue.begin(), mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });


}

std::deque<Process*> RoundRobin::mainqueue;
std::deque<Process*> RoundRobin::ready;
Process* RoundRobin::current_process=nullptr;
Process* RoundRobin::running_process=nullptr;
bool RoundRobin:: stop=false;
RoundRobin::~RoundRobin() {
    qDebug() << "RoundRobin destructor";
}

void RoundRobin::checkArrival() {
    while (!mainqueue.empty()) {
        Process* P = mainqueue.front();
        if (P->getArrivalTime() <= current_time) {
            ready.push_back(P);
            // qDebug()<< "P "<<current_process->getProcessNumber()<<" ready at : "<<current_time;
            mainqueue.pop_front(); }
        else {
            break; }
    }
}


void RoundRobin::schedule() {
    if (stop)
    return;
    std::thread t(&RoundRobin::Roundrobin, this, timeQuantum);
    t.detach();

}

void RoundRobin::addProcessRR(Process* p) {

    mainqueue.push_back(p);

}

void RoundRobin::addNewProcessRR(Process* p) {
    qDebug() << "new process added : " << p->getProcessNumber();

    mainqueue.push_back(p);

    std::sort(mainqueue.begin(), mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });
    // Processes.push_back(*p);

}

int completed = 0;
float avg_waiting_time = 0;
float avg_turnaround_time = 0;
int turnaround_time = 0;
int waiting_time = 0;
int pid=-1;
void RoundRobin:: Roundrobin(int Q) {

    // checkArrival();
    // if (!ready.empty()) {
    //     current_process = ready.front();
    //     running_process = current_process;
    //     pid = current_process->getProcessNumber();
    //     emit dataChanged(pid);
    // }

    int valid = 0;
    //completed < Processes.size()
    while (!stop) {

        checkArrival();
        if (ready.empty()) {
            pid=-1;
            emit dataChanged(pid);
            current_time++;
            waitOneSecond();
            continue;
        }
        current_process = ready.front();
        if (valid == timeQuantum && current_process->getRemainingTime() > 0) {
            ready.pop_front();
            ready.push_back(current_process);
            current_process = ready.front();
            valid = 0;
        }



        if (valid < timeQuantum) {
            running_process=current_process;
            current_process->decrementRemainingTime();
            int rem = current_process->getRemainingTime();
            pid=current_process->getProcessNumber();
            emit dataChanged(pid);

            qDebug()<<"remaining : " <<rem;
            valid++;
            current_time++;
            waitOneSecond();


            if (current_process->getRemainingTime() == 0) {
                ready.pop_front();
                turnaround_time = current_time - current_process->getArrivalTime();
                waiting_time = turnaround_time - current_process->getBurstTime();
                current_process->setTurnaroundTime(turnaround_time);
                current_process->setWaitingTime(waiting_time);
                emit  ProcessFinished(current_process->getProcessNumber(), waiting_time, turnaround_time);

                current_process=nullptr;
                running_process=nullptr;
                pid=-1;
                //qDebug()<< "P "<<current_process->getProcessNumber()<<"terminated at : "<<current_time;

                avg_turnaround_time += turnaround_time;
                avg_waiting_time += waiting_time;
                valid = 0;
                completed++;
            }



        }



    }
    // qDebug() << "Average waiting time = " << avg_waiting_time / Processes.size();
    // qDebug() << "Average turnaround time = " << avg_turnaround_time / Processes.size();
}

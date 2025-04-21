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
Process* Scheduler::running_process = nullptr;
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

// void RoundRobin::stopwatch() {
//     while (!finished) {
//         current_time++;
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
// }

// void RoundRobin::livetable() {
//     qDebug() << "Starting livetable thread";
//     while (!finished) {
//         if (current_process) {
//             int rem = std::min(current_process->getRemainingTime(), timeQuantum);
//             for (int i = 0; i < rem && !finished; ++i) {
//                 current_process->decrementRemainingTime();
//                 emit dataChanged(current_process->getProcessNumber());
//                 std::this_thread::sleep_for(std::chrono::seconds(1));
//                 qDebug() << "Remaining Time: " << current_process->getRemainingTime();
//             }
//         } else {
//             std::this_thread::sleep_for(std::chrono::milliseconds(100));
//         }
//     }
// }

void RoundRobin:: schedule() {
    std::thread t(&RoundRobin::Roundrobin, this, timeQuantum);
    t.detach();

}
std::mutex mux;

void RoundRobin::addProcessRR(Process* p) {

    mainqueue.push_back(p);

}

void RoundRobin::addNewProcessRR(Process* p) {
    qDebug() << "new process added : " << p->getProcessNumber();

    mainqueue.push_back(p);

    std::sort(mainqueue.begin(), mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });
    Processes.push_back(*p);

}

int completed = 0;
float avg_waiting_time = 0;
float avg_turnaround_time = 0;
int turnaround_time = 0;
int waiting_time = 0;

void RoundRobin:: Roundrobin(int Q) {

    qDebug() << "number of processes : " << mainqueue.size();


    int valid = 0;
    //completed < Processes.size()
    while (true) {

        checkArrival();
        if (ready.empty()) {
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
            waitOneSecond();
            current_process->decrementRemainingTime();
            int rem = current_process->getRemainingTime();
            emit dataChanged(current_process->getProcessNumber());

            qDebug()<<"remaining : " <<rem;
            valid++;

            current_time++;


            if (current_process->getRemainingTime() == 0) {
                ready.pop_front();
                turnaround_time = current_time - current_process->getArrivalTime();
                waiting_time = turnaround_time - current_process->getBurstTime();
                current_process->setTurnaroundTime(turnaround_time);
                current_process->setWaitingTime(waiting_time);
                emit  ProcessFinished(current_process->getProcessNumber(), waiting_time, turnaround_time);

                current_process=nullptr;
                running_process=nullptr;
                //qDebug()<< "P "<<current_process->getProcessNumber()<<"terminated at : "<<current_time;

                avg_turnaround_time += turnaround_time;
                avg_waiting_time += waiting_time;
                valid = 0;
                completed++;
            }
        }



    }
    qDebug() << "Average waiting time = " << avg_waiting_time / Processes.size();
    qDebug() << "Average turnaround time = " << avg_turnaround_time / Processes.size();
}

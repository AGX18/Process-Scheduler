#include "roundrobin.h"

RoundRobin::RoundRobin(QObject *parent, std::vector<Process> processes, int timeQ)
    : Scheduler{parent}, processes(processes), timeQ(timeQ), totalWaitingTime(0)
    , totalTurnaroundTime(0), currentProcess(nullptr), remainingExecTimeForProcess(0), currentTime(0), indexArrived(0)
{
    // sort them in ascending order according to the arrival time
    std::sort(this->processes.begin(), this->processes.end(), [](const Process &a, const Process &b) {
        return a.getArrivalTime() < b.getArrivalTime();
    });

    this->schedulerTimer = new QTimer(this);
    // QThread::sleep(1);  // Sleep for 1 second
    connect(schedulerTimer, &QTimer::timeout, this, &RoundRobin::schedule);
    schedulerTimer->start(1000);  // 1 second interval
}

RoundRobin::~RoundRobin()
{
    delete this->schedulerTimer;
    // Make sure to delete any dynamically allocated processes (if any) here to avoid memory leaks
}

void RoundRobin::schedule() {
    qDebug() << "Scheduler tick! Current time: " << this->currentTime;


    // Check if a process has arrived
    for (int i = indexArrived; i < processes.size(); ++i) {
        if (processes[i].getArrivalTime() <= this->currentTime) {
            qDebug() << processes[i].getArrivalTime();
            qDebug() << processes[i].getBurstTime();

            arrivedQueue.push_back(&processes[i]);
            indexArrived++;
        } else {
            break;  // Since processes are sorted by arrival time
        }
    }

    // If no process has arrived and there is no process that is running , do nothing
    if (arrivedQueue.empty() && this->currentProcess == nullptr) {
        this->currentTime++;
        return;
    }

    // Check if a process has been chosen or if we need to pick a new one
    if (this->currentProcess == nullptr || this->remainingExecTimeForProcess == 0) {
        // If the current process still has remaining time, move it back to the arrived queue
        if (this->currentProcess != nullptr && this->currentProcess->getRemainingTime() != 0) {
            arrivedQueue.push_back(this->currentProcess);  // Do not delete the current process here!
        }

        // Assign a new process from the arrived queue
        if (!arrivedQueue.empty()) {
            this->currentProcess = arrivedQueue.front();
            arrivedQueue.pop_front();
            this->remainingExecTimeForProcess = this->timeQ;  // Reset the time quantum
        }

    }

    // Emit signal to update data
    emit dataChanged(this->currentProcess->getProcessNumber());
    qDebug() << "decrementing the remaining time of the process: P" << this->currentProcess->getProcessNumber();
    // Decrement the process's remaining time and the time quantum
    this->currentProcess->decrementRemainingTime();
    qDebug() << "remaining time is: " << this->currentProcess->getRemainingTime();
    this->remainingExecTimeForProcess--;

    // If the process has finished, calculate turnaround and waiting times
    if (this->currentProcess->getRemainingTime() == 0) {
        int turnaround = (this->currentTime + 1)- this->currentProcess->getArrivalTime();
        int waiting = turnaround - this->currentProcess->getBurstTime();
        this->totalWaitingTime += waiting;
        this->totalTurnaroundTime += turnaround;
        emit ProcessFinished(this->currentProcess->getProcessNumber(), waiting, turnaround);
        this->currentProcess = nullptr;
    }

    this->currentTime++;  // Increment the clock
}

void RoundRobin::addNewProcess(Process *p)
{
    qDebug() << "Adding a new Process" << p->getProcessNumber();
    processes.push_back(*p);  // Add the new process to the list


}

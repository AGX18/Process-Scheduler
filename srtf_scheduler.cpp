#include "srtf_scheduler.h"
#include <algorithm>
#include <climits>

SRTFScheduler::SRTFScheduler(QObject *parent, std::vector<Process> processes)
    : Scheduler(parent), processes(processes), totalWaitingTime(0), totalTurnaroundTime(0), schedulerTimer(nullptr), currentTime(0), indexArrived(0), currentProcess(nullptr)
{
    std::sort(this->processes.begin(), this->processes.end(), [](const Process &a, const Process &b) {
        return a.getArrivalTime() < b.getArrivalTime();
    });
    this->schedulerTimer = new QTimer(this);
    connect(schedulerTimer, &QTimer::timeout, this, &SRTFScheduler::schedule);
    schedulerTimer->start(1000);
}

SRTFScheduler::~SRTFScheduler() {
    delete this->schedulerTimer;
}

void SRTFScheduler::schedule() {
    // Check if a process has arrived
    for (int i = indexArrived; i < processes.size(); ++i) {
        if (processes[i].getArrivalTime() <= currentTime) {
            arrivedQueue.push_back(&processes[i]);
            indexArrived++;
        } else {
            break;
        }
    }

    if (arrivedQueue.empty() && this->currentProcess == nullptr) {
        currentTime++;
        return;
    }

    // Find the process with the shortest remaining time
    Process* shortest = nullptr;
    int shortestRemaining = INT_MAX;
    for (Process* proc : arrivedQueue) {
        if (proc->getRemainingTime() > 0 && proc->getRemainingTime() < shortestRemaining) {
            shortestRemaining = proc->getRemainingTime();
            shortest = proc;
        }
    }

    if (shortest == nullptr) {
        currentTime++;
        return;
    }

    currentProcess = shortest;

    if (currentProcess->getStartTime() == -1) {
        currentProcess->setStartTime(currentTime);
    }

    emit dataChanged(currentProcess->getProcessNumber());
    currentProcess->decrementRemainingTime();
    currentTime++;

    if (currentProcess->getRemainingTime() == 0) {
        currentProcess->setFinishTime(currentTime);
        currentProcess->setTurnaroundTime(currentTime - currentProcess->getArrivalTime());
        currentProcess->setWaitingTime(currentProcess->getTurnaroundTime() - currentProcess->getBurstTime());
        totalWaitingTime += currentProcess->getWaitingTime();
        totalTurnaroundTime += currentProcess->getTurnaroundTime();
        emit ProcessFinished(currentProcess->getProcessNumber(), currentProcess->getWaitingTime(), currentProcess->getTurnaroundTime());
        // Remove finished process from arrivedQueue
        auto it = std::find(arrivedQueue.begin(), arrivedQueue.end(), currentProcess);
        if (it != arrivedQueue.end()) {
            arrivedQueue.erase(it);
        }
        currentProcess = nullptr;
    }
}

void SRTFScheduler::addNewProcess(Process* p) {
    this->processes.emplace_back(*p);
    std::sort(this->processes.begin(), this->processes.end(), [](const Process &a, const Process &b) {
        return a.getArrivalTime() < b.getArrivalTime();
    });
}

void SRTFScheduler::printResults() {
    float avgTurnaround = processes.empty() ? 0 : (float)totalTurnaroundTime / processes.size();
    float avgWaiting = processes.empty() ? 0 : (float)totalWaitingTime / processes.size();
    qDebug() << "Average turnaround time:" << avgTurnaround;
    qDebug() << "Average waiting time:" << avgWaiting;
}

#include "PreemptivePriorityScheduling.h"
#include <algorithm>
#include <QDebug>
#include "mainwindow.h"

PreemptivePriorityScheduler::PreemptivePriorityScheduler(QObject *parent, std::vector<Process> Processes)
    : Scheduler{parent, std::move(Processes)}, currentTime(0), totalWaitingTime(0), totalTurnaroundTime(0), completedProcesses(0)
{
    qInfo() << this << "constructed Preemptive Priority Scheduler";
}

void PreemptivePriorityScheduler::schedule()
{
    while (completedProcesses < Processes.size()) {
        sortProcessesByPriority();

        Process* processToRun = nullptr;


        for (int i = 0; i < Processes.size(); ++i) {
            if (Processes[i].getArrivalTime() <= currentTime && Processes[i].getRemainingTime() > 0) {
                if (processToRun == nullptr || Processes[i].getPriority() < processToRun->getPriority()) {
                    processToRun = &Processes[i];
                }
            }
        }


        if (processToRun != nullptr) {

            processToRun->decrementRemainingTime();

            if (processToRun->getRemainingTime() == 0) {

                int turnaroundTime = currentTime + 1 - processToRun->getArrivalTime();
                int waitingTime = turnaroundTime - processToRun->getBurstTime();
                processToRun->setTurnaroundTime(turnaroundTime);
                processToRun->setWaitingTime(waitingTime);

                totalTurnaroundTime += turnaroundTime;
                totalWaitingTime += waitingTime;
                completedProcesses++;

                emit dataChanged(processToRun->getProcessNumber());
                emit ProcessFinished(processToRun->getProcessNumber(), waitingTime, turnaroundTime);
            } else {
                emit dataChanged(processToRun->getProcessNumber());
            }


            currentTime++;
            waitOneSecond();
            checkForArrival();
            updateWaitingTimes();
        } else {
            qWarning() << "Idle at time" << currentTime;
            currentTime++;
            waitOneSecond();
            checkForArrival();
            updateWaitingTimes();
        }
    }

    qInfo() << "All processes finished.";
    if (completedProcesses > 0) {
        float avgWaitingTime = static_cast<float>(totalWaitingTime) / completedProcesses;
        float avgTurnaroundTime = static_cast<float>(totalTurnaroundTime) / completedProcesses;

        qInfo() << "Average waiting time = " << avgWaitingTime;
        qInfo() << "Average turnaround time = " << avgTurnaroundTime;
    }
}

void PreemptivePriorityScheduler::sortProcessesByPriority()
{
    std::sort(Processes.begin(), Processes.end(), [](const Process& a, const Process& b) {
        return a.getPriority() < b.getPriority();
    });
}

void PreemptivePriorityScheduler::checkForArrival()
{
    for (auto& p : Processes) {
        if (p.getArrivalTime() <= currentTime && p.getRemainingTime() > 0 && p.getStartTime() == -1) {
            p.setStartTime(currentTime);
            emit dataChanged(p.getProcessNumber());
        }
    }
}

void PreemptivePriorityScheduler::updateWaitingTimes()
{
    for (auto& process : Processes) {
        if (process.getRemainingTime() > 0 && process.getArrivalTime() <= currentTime && process.getStartTime() != -1) {
            process.setWaitingTime(process.getWaitingTime() + 1);
        }
    }
}

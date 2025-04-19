#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "scheduler.h"
#include "process.h"
#include <algorithm>

class FCFSScheduler : public Scheduler
{
    Q_OBJECT

public:
    explicit FCFSScheduler(QObject* parent = nullptr, std::vector<Process>* processes = nullptr)
        : Scheduler(parent, processes) {
    }

    void schedule() override {
        if (!processes || processes->empty()) return;

        // Sort processes by arrival time (FCFS)
        std::sort(processes->begin(), processes->end(),
            [](const Process& a, const Process& b) {
                return a.getArrivalTime() < b.getArrivalTime();
            });

        for (auto& process : *processes) {
            // If no process is available yet, jump to arrival time
            if (process.getArrivalTime() > currentTime) {
                currentTime = process.getArrivalTime();
            }

            process.setStartTime(currentTime);

            // Execute process one second at a time
            while (process.getRemainingTime() > 0) {
                emit dataChanged(process.getProcessNumber());
                waitOneSecond();
                process.decrementRemainingTime();
                currentTime++;
            }

            process.setFinishTime(currentTime);
            process.setTurnaroundTime(currentTime - process.getArrivalTime());
            process.setWaitingTime(process.getStartTime() - process.getArrivalTime());

            emit processFinished(process.getProcessNumber(),
                process.getWaitingTime(),
                process.getTurnaroundTime());
        }

        printResults();
    }

private:
    void printResults() {
        float totalWaiting = 0, totalTurnaround = 0;
        for (const auto& p : *processes) {
            totalWaiting += p.getWaitingTime();
            totalTurnaround += p.getTurnaroundTime();
        }

        qDebug() << "FCFS Results:";
        qDebug() << "Average turnaround time:" << totalTurnaround / processes->size();
        qDebug() << "Average waiting time:" << totalWaiting / processes->size();
    }
};

#endif // FCFS_SCHEDULER_H
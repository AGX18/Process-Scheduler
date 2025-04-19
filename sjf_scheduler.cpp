#ifndef SJF_SCHEDULER_H
#define SJF_SCHEDULER_H

#include "scheduler.h"
#include "process.h"
#include <algorithm>
#include <climits>

class SJFScheduler : public Scheduler
{
    Q_OBJECT

public:
    explicit SJFScheduler(QObject* parent = nullptr, std::vector<Process>* processes = nullptr)
        : Scheduler(parent, processes) {
    }

    void schedule() override {
        if (!processes || processes->empty()) return;

        std::vector<bool> executed(processes->size(), false);
        int completed = 0;

        while (completed < processes->size()) {
            int shortestIndex = -1;
            int shortestBurst = INT_MAX;
            int earliestArrival = INT_MAX;

            // Find shortest job that has arrived
            for (size_t i = 0; i < processes->size(); i++) {
                if (!executed[i] && (*processes)[i].getArrivalTime() <= currentTime) {
                    if ((*processes)[i].getBurstTime() < shortestBurst) {
                        shortestBurst = (*processes)[i].getBurstTime();
                        earliestArrival = (*processes)[i].getArrivalTime();
                        shortestIndex = i;
                    }
                    else if ((*processes)[i].getBurstTime() == shortestBurst) {
                        // Tie-breaker: earlier arrival time
                        if ((*processes)[i].getArrivalTime() < earliestArrival) {
                            earliestArrival = (*processes)[i].getArrivalTime();
                            shortestIndex = i;
                        }
                    }
                }
            }

            if (shortestIndex == -1) {
                currentTime++;
                continue;
            }

            auto& process = (*processes)[shortestIndex];
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

            executed[shortestIndex] = true;
            completed++;
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

        qDebug() << "Average turnaround time:" << totalTurnaround / processes->size();
        qDebug() << "Average waiting time:" << totalWaiting / processes->size();
    }
};

#endif // SJF_SCHEDULER_H
#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include "scheduler.h"
#include "process.h"
#include <algorithm>
#include <climits>

class PriorityScheduler : public Scheduler
{
    Q_OBJECT

public:
    explicit PriorityScheduler(QObject* parent = nullptr, std::vector<Process>* processes = nullptr)
        : Scheduler(parent, processes) {
    }

    void schedule() override {
        if (!processes || processes->empty()) return;

        std::vector<bool> executed(processes->size(), false);
        int completed = 0;

        while (completed < processes->size()) {
            int highestPriorityIndex = -1;
            int highestPriority = INT_MAX;
            int earliestArrival = INT_MAX;

            // Find highest priority (lowest number) process that has arrived
            for (size_t i = 0; i < processes->size(); i++) {
                if (!executed[i] && (*processes)[i].getArrivalTime() <= currentTime) {
                    if ((*processes)[i].getPriority() < highestPriority) {
                        highestPriority = (*processes)[i].getPriority();
                        earliestArrival = (*processes)[i].getArrivalTime();
                        highestPriorityIndex = i;
                    }
                    else if ((*processes)[i].getPriority() == highestPriority) {
                        // Tie-breaker: earlier arrival time
                        if ((*processes)[i].getArrivalTime() < earliestArrival) {
                            earliestArrival = (*processes)[i].getArrivalTime();
                            highestPriorityIndex = i;
                        }
                    }
                }
            }

            if (highestPriorityIndex == -1) {
                currentTime++;
                continue;
            }

            auto& process = (*processes)[highestPriorityIndex];
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

            executed[highestPriorityIndex] = true;
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

#endif // PRIORITY_SCHEDULER_H
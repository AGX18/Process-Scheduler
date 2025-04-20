#ifndef SRTF_SCHEDULER_H
#define SRTF_SCHEDULER_H

#include "scheduler.h"
#include "process.h"
#include <algorithm>
#include <climits>

class SRTFScheduler : public Scheduler
{
    Q_OBJECT

public:
    explicit SRTFScheduler(QObject* parent = nullptr, std::vector<Process>* processes = nullptr)
        : Scheduler(parent, processes) {
    }

    void schedule() override {
        if (!processes || processes->empty()) return;

        int completed = 0;
        int n = processes->size();
        std::vector<bool> finished(n, false);

        while (completed < n) {
            int shortestIndex = -1;
            int shortestRemaining = INT_MAX;

            for (int i = 0; i < n; ++i) {
                auto& p = (*processes)[i];
                if (!finished[i] && p.getArrivalTime() <= currentTime && p.getRemainingTime() > 0) {
                    if (p.getRemainingTime() < shortestRemaining) {
                        shortestRemaining = p.getRemainingTime();
                        shortestIndex = i;
                    }
                    else if (p.getRemainingTime() == shortestRemaining) {
                        if (p.getArrivalTime() < (*processes)[shortestIndex].getArrivalTime()) {
                            shortestIndex = i;
                        }
                    }
                }
            }

            if (shortestIndex == -1) {
                currentTime++;
                continue;
            }

            auto& currentProcess = (*processes)[shortestIndex];
            if (currentProcess.getStartTime() == -1) {
                currentProcess.setStartTime(currentTime);
            }

            emit dataChanged(currentProcess.getProcessNumber());
            waitOneSecond();
            currentProcess.decrementRemainingTime();
            currentTime++;

            if (currentProcess.getRemainingTime() == 0) {
                currentProcess.setFinishTime(currentTime);
                currentProcess.setTurnaroundTime(currentTime - currentProcess.getArrivalTime());
                currentProcess.setWaitingTime(currentProcess.getTurnaroundTime() - currentProcess.getBurstTime());

                finished[shortestIndex] = true;
                completed++;

                emit processFinished(currentProcess.getProcessNumber(),
                                     currentProcess.getWaitingTime(),
                                     currentProcess.getTurnaroundTime());
            }
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

#endif // SRTF_SCHEDULER_H

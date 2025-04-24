#include "PreemptivePriorityScheduling.h"
#include <algorithm>
#include <QThread>

void PreemptivePriorityScheduler::schedule() {
    int currentTime = 0;
    QList<Process*> finishedProcesses;

    while (!processes.isEmpty() || !readyQueue.isEmpty()) {
        // Add arrived processes to ready queue
        for (int i = 0; i < processes.size(); ++i) {
            if (processes[i]->getArrivalTime() <= currentTime) {
                readyQueue.append(processes[i]);
                processes.removeAt(i);
                --i;
            }
        }

        // If ready queue not empty, execute process with highest priority
        if (!readyQueue.isEmpty()) {
            std::sort(readyQueue.begin(), readyQueue.end(), [](Process* a, Process* b) {
                return a->getPriority() < b->getPriority();  // smaller number = higher priority
            });

            Process* currentProcess = readyQueue.first();

            if (currentProcess->getStartTime() == -1)
                currentProcess->setStartTime(currentTime);

            emit dataChanged(currentProcess->getProcessNumber(), currentTime);

            currentProcess->decrementRemainingTime();
            waitOneSecond();
            ++currentTime;

            if (currentProcess->getRemainingTime() == 0) {
                currentProcess->setFinishTime(currentTime);
                currentProcess->calculateTimes();
                finishedProcesses.append(currentProcess);
                emit ProcessFinished(currentProcess);
            }
        } else {
            // No ready process → CPU is idle
            emit dataChanged(-1, currentTime);
            waitOneSecond();
            ++currentTime;
        }
    }

    // حساب المتوسطات بعد الانتهاء
    double totalTurnaround = 0;
    double totalWaiting = 0;
    for (Process* p : finishedProcesses) {
        totalTurnaround += p->getTurnaroundTime();
        totalWaiting += p->getWaitingTime();
    }

    double avgTurnaround = totalTurnaround / finishedProcesses.size();
    double avgWaiting = totalWaiting / finishedProcesses.size();

    emit averagesCalculated(avgTurnaround, avgWaiting);
}

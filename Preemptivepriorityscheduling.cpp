#include "PreemptivePriorityScheduling.h"
#include <QDebug>
#include <thread>
#include <algorithm>
#include <chrono>

// static queues shared by all instances
std::deque<Process*> PreemptivePriorityScheduler::mainqueue;
std::deque<Process*> PreemptivePriorityScheduler::readyQueue;

// في cpp
PreemptivePriorityScheduler::PreemptivePriorityScheduler(QObject* parent,
                                                         const std::vector<Process>& Processes)
    : Scheduler(parent, Processes)    // الآن النوع يطابق exactly منشئ الأب
{
    current_time = 0;
    // نخلق مؤشرات على العناصر المحفوظة في الـ base-class container
    for (Process& proc : this->Processes) {
        Process* p = &proc;
        mainqueue.push_back(p);
    }
    std::sort(mainqueue.begin(), mainqueue.end(),
              [](Process* a, Process* b){
                  return a->getArrivalTime() < b->getArrivalTime();
              });
}


PreemptivePriorityScheduler::~PreemptivePriorityScheduler() {
    qDebug() << "PreemptivePriorityScheduler destructor";
}

void PreemptivePriorityScheduler::addNewProcess(Process* p) {
    qDebug() << "PPS: new process added:" << p->getProcessNumber();
    // add to main queue and re-sort by arrival
    mainqueue.push_back(p);
    std::sort(mainqueue.begin(), mainqueue.end(),
              [](Process* a, Process* b){
                  return a->getArrivalTime() < b->getArrivalTime();
              });
    // also track in base class container
    Processes.push_back(p);
}

void PreemptivePriorityScheduler::schedule() {
    // run the preemptive priority loop in its own std::thread
    std::thread t(&PreemptivePriorityScheduler::runPriority, this);
    t.detach();
}

void PreemptivePriorityScheduler::checkArrival() {
    // move any newly-arrived processes into readyQueue
    while (!mainqueue.empty()) {
        Process* p = mainqueue.front();
        if (p->getArrivalTime() <= current_time) {
            readyQueue.push_back(p);
            mainqueue.pop_front();
        } else {
            break;
        }
    }
}

void PreemptivePriorityScheduler::runPriority() {
    QList<Process*> finished;
    while (!mainqueue.empty() || !readyQueue.empty()) {
        checkArrival();

        if (readyQueue.empty()) {
            // CPU idle
            emit dataChanged(-1);
            waitOneSecond();
            ++current_time;
            continue;
        }

        // pick highest priority (smallest priority value)
        std::sort(readyQueue.begin(), readyQueue.end(),
                  [](Process* a, Process* b){
                      return a->getPriority() < b->getPriority();
                  });
        Process* cur = readyQueue.front();

        if (cur->getStartTime() == -1)
            cur->setStartTime(current_time);

        // signal GUI that this process ran one unit
        emit dataChanged(cur->getProcessNumber());

        // execute one time unit
        cur->decrementRemainingTime();
        waitOneSecond();
        ++current_time;

        if (cur->getRemainingTime() == 0) {
            // compute times
            cur->setFinishTime(current_time);
            int tat = cur->getFinishTime() - cur->getArrivalTime();
            int wt  = tat - cur->getBurstTime();
            cur->setTurnaroundTime(tat);
            cur->setWaitingTime(wt);

            // remove from ready queue
            readyQueue.pop_front();
            finished.push_back(cur);

            emit ProcessFinished(cur->getProcessNumber(), wt, tat);
        }
    }

    // compute averages
    double sumT = 0, sumW = 0;
    for (Process* p : finished) {
        sumT += p->getTurnaroundTime();
        sumW += p->getWaitingTime();
    }
    double avgT = sumT / finished.size();
    double avgW = sumW / finished.size();
    emit averagesCalculated(avgT, avgW);
}

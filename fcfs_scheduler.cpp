#include "fcfs_scheduler.h"

FCFSScheduler::FCFSScheduler(QObject *parent, std::vector<Process> processes)
    : Scheduler(parent), processes(processes), currentProcess(nullptr),
    currentTime(0), indexArrived(0), running(true)
{
    // Sort processes by arrival time (FCFS)
    std::sort(this->processes.begin(), this->processes.end(),
              [](const Process &a, const Process &b) {
                  return a.getArrivalTime() < b.getArrivalTime();
              });

    this->schedulerTimer = new QTimer(this);
    connect(schedulerTimer, &QTimer::timeout, this, &FCFSScheduler::schedulerTick);
    schedulerTimer->start(1000);  // 1 second interval
}

FCFSScheduler::~FCFSScheduler()
{
    stopScheduler();
    delete schedulerTimer;
}

void FCFSScheduler::stopScheduler()
{
    running = false;
    if (schedulerTimer->isActive()) {
        schedulerTimer->stop();
    }
}

void FCFSScheduler::schedule()
{
    // Start the timer-based scheduling
    schedulerTimer->start(1000);
}

void FCFSScheduler::addNewProcess(Process* p)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    qDebug() << "Adding new process P" << p->getProcessNumber()
             << " with arrival time " << p->getArrivalTime();

    // Add to main processes list
    processes.push_back(*p);
    delete p;

    // If the process has already arrived, add to queue
    if (p->getArrivalTime() <= currentTime) {
        arrivedQueue.push_back(&processes.back());
    }
}

void FCFSScheduler::schedulerTick()
{
    if (!running) return;

    qDebug() << "FCFS Scheduler tick! Current time: " << currentTime;

    std::unique_lock<std::mutex> lock(queueMutex);

    // Check for newly arrived processes
    for (int i = indexArrived; i < processes.size(); ++i) {
        if (processes[i].getArrivalTime() <= currentTime) {
            arrivedQueue.push_back(&processes[i]);
            indexArrived++;
            qDebug() << "Process P" << processes[i].getProcessNumber()
                     << " arrived at time " << currentTime;
        } else {
            break;  // Processes are sorted by arrival time
        }
    }
    lock.unlock();

    // If no process is running, pick the next one from the queue
    if (currentProcess == nullptr && !arrivedQueue.empty()) {
        currentProcess = arrivedQueue.front();
        arrivedQueue.pop_front();
        currentProcess->setStartTime(currentTime);
        qDebug() << "Started executing P" << currentProcess->getProcessNumber();
    }

    // Execute the current process
    if (currentProcess != nullptr) {
        emit dataChanged(currentProcess->getProcessNumber());
        currentProcess->decrementRemainingTime();
        qDebug() << "Executing P" << currentProcess->getProcessNumber()
                 << ", remaining time: " << currentProcess->getRemainingTime();

        // Check if process finished
        if (currentProcess->getRemainingTime() == 0) {
            int turnaround = currentTime + 1 - currentProcess->getArrivalTime();
            int waiting = currentProcess->getStartTime() - currentProcess->getArrivalTime();

            emit ProcessFinished(currentProcess->getProcessNumber(), waiting, turnaround);
            qDebug() << "Process P" << currentProcess->getProcessNumber()
                     << " finished. WT: " << waiting << ", TAT: " << turnaround;

            currentProcess = nullptr;
        }
    }

    currentTime++;
}

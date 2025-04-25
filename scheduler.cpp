#include "scheduler.h"

Scheduler::Scheduler(QObject *parent,  std::vector<Process> Processes)
    : QObject{parent}, Processes(std::move(Processes))
{
    qInfo() << this << "constructed" << QThread::currentThread();

}

// Define the static variables once

Process* Scheduler::current_process = nullptr;
Process* Scheduler::running_process = nullptr;

void Scheduler::waitOneSecond()
{
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 1000) {
        // Busy waiting
    }
}

Scheduler::~Scheduler()
{
    qInfo() << this << "deconstructed" << QThread::currentThread();
}

void Scheduler::addNewProcess(Process* p)
{
    this->Processes.emplace_back(*p);
    qDebug() << p->getProcessNumber() << "adding a new process";
}

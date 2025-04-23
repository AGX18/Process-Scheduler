#include "scheduler.h"
#include "mainwindow.h"
Scheduler::Scheduler(QObject *parent,  std::vector<Process> Processes)
    : QObject{parent}, Processes(std::move(Processes))
{
    qInfo() << this << "constructed" << QThread::currentThread();
}
QString Scheduler::SchedulerRR = nullptr;
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
{     if(SchedulerRR=="Round Robin"){
        qDebug() << p->getProcessNumber() << "adding a new process in RR";
        RoundRobin::addNewProcessRR(p);
    }

    this->Processes.emplace_back(*p);
    qDebug() << p->getProcessNumber() << "adding a new process";
}

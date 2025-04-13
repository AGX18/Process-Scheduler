#include "scheduler.h"

Scheduler::Scheduler(QObject *parent, std::vector<Process>* Processes)
    : QObject{parent}, Processes(Processes)
{
    qInfo() << this << "constructed" << QThread::currentThread();
}

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

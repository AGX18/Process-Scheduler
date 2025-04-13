#include "scheduler.h"

Scheduler::Scheduler(QObject *parent)
    : QObject{parent}
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

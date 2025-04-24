#pragma once
#include "scheduler.h"
#include "mainwindow.h"
#include "process.h"
#include <QTimer>
#include <deque>

class RoundRobin : public Scheduler
{
    Q_OBJECT
public:
    RoundRobin(QObject *parent, std::vector<Process> Processes, int quantum);
    ~RoundRobin();
    static std::deque<Process*> ready;
    static std::deque<Process*> mainqueue;
    // static Process* current_process;
    //  static Process* running_process;


public slots:
    void schedule() override;
    static void addProcessRR(Process* p);
    void addNewProcessRR(Process *P);


private:
    void Roundrobin(int Q);
    void checkArrival();
    // void incrementTime();
    // void stopwatch();
    // void startArrivalThread();
    // void livetable();

    int current_time = 0;
    QTimer *timer;
    int timeQuantum;
    bool finished = false;
    //Process* current_process;
    QVector<bool> added;
    // static std::mutex queueMutex;
    // static    std::condition_variable cv;

    // std::atomic<bool> stopwatchRunning = true;
    // std::thread stopwatchThread;
};

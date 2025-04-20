#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "process.h"
#include <vector>

/**
 * @brief The Scheduler class
 * inherit from this class
 * and implement the schedule functionality
 */
class Scheduler : public QObject
{
    Q_OBJECT

protected:
    std::vector<Process> Processes;

public:
    explicit Scheduler(QObject *parent = nullptr, std::vector<Process> Processes = {});
    /**
     * @brief schedule
     * the function that changes the data and
     * send a signal to the main Window to reflect those changes in the shown screen
     */

    /**
     * @brief waitOneSecond
     * you can use this function or just add `QThread::currentThread()->msleep(1000);`
     */
    void waitOneSecond();

    virtual ~Scheduler();

signals:
    /**
     * @brief dataChanged
     * the signal that gets emited to the main window when data is changed
     */
    void dataChanged(int processID);
    void ProcessFinished(int processID,int waitingTime, int TurnaroundTime);
// emit datachanged(Processid);
// emit ProcessFinshed(pid, waitingtime, turnaround time)

public slots:
    virtual void schedule() = 0;
    virtual void addNewProcess(Process* p);

};

#endif // SCHEDULER_H

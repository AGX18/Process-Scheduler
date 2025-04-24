#include "fcfs_scheduler.h"

std::deque<Process*> FCFSScheduler::fcfs_mainqueue;
std::deque<Process*> FCFSScheduler::fcfs_ready;


FCFSScheduler::FCFSScheduler(QObject *parent, std::vector<Process> Processes)
    : Scheduler(parent, Processes), current_time(0), avg_turnaround_time(0),
    avg_waiting_time(0), completed(0)
{
    std::sort(fcfs_mainqueue.begin(), fcfs_mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });
}

void FCFSScheduler::addNewProcessFCFS(Process* p)
{
    qDebug() << "New process added: P" << p->getProcessNumber();
    Processes.push_back(*p);
    std::sort(fcfs_mainqueue.begin(), fcfs_mainqueue.end(), [](Process* a, Process* b) {
        return a->getArrivalTime() < b->getArrivalTime();
    });
}

FCFSScheduler::~FCFSScheduler()
{
    qDebug() << "FCFS destructor";
}

void FCFSScheduler::checkArrival()
{
    while (! fcfs_mainqueue.empty()) {
        Process* P =  fcfs_mainqueue.front();
        if (P->getArrivalTime() <= current_time) {
           fcfs_ready.push_back(P);
           fcfs_mainqueue.pop_front();
        } else {
            break;
        }
    }
}

void FCFSScheduler::schedule()
{
    std::thread t(&FCFSScheduler::RUNFCFS, this);
    t.detach();
}

void FCFSScheduler::addProcessFCFS(Process* p)
{

    fcfs_mainqueue.push_back(p);
}

void FCFSScheduler::RUNFCFS()
{
    qDebug() << "Number of processes: " << fcfs_mainqueue.size();

    while (completed < Processes.size()) {
        checkArrival();

        if (fcfs_ready.empty()) {
            current_time++;
            waitOneSecond();
            continue;
        }

        Process* current_process = fcfs_ready.front();
        running_process = current_process;

             waitOneSecond();  // Simulate 1-second execution

        if (current_process->getRemainingTime() > 0) {
            current_process->decrementRemainingTime();

            emit dataChanged(current_process->getProcessNumber());
            qDebug() << "Executing P" << current_process->getProcessNumber()
                     << ", remaining time: " << current_process->getRemainingTime();

            current_time++;

            if (current_process->getRemainingTime() == 0) {
                fcfs_ready.pop_front();

                int turnaround_time = current_time - current_process->getArrivalTime();
                int waiting_time = turnaround_time - current_process->getBurstTime();

                current_process->setTurnaroundTime(turnaround_time);
                current_process->setWaitingTime(waiting_time);
                current_process->setRemainingTime(0); // Just in case

                emit ProcessFinished(current_process->getProcessNumber(), waiting_time, turnaround_time);
                qDebug() << "Process P" << current_process->getProcessNumber()
                         << " finished. WT: " << waiting_time << ", TAT: " << turnaround_time;

                avg_turnaround_time += turnaround_time;
                avg_waiting_time += waiting_time;
                completed++;

                current_process = nullptr;
                running_process = nullptr;
            }
        }

    }

    qDebug() << "Average waiting time = " << (avg_waiting_time / completed);
    qDebug() << "Average turnaround time = " << (avg_turnaround_time / completed);

}

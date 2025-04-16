#ifndef PROCESS_H
#define PROCESS_H

class Process
{
public:
    Process(int processNumber, int arrivalTime, int burstTime, int priority);

    int getProcessNumber();


    int getArrivalTime();

    void setArrivalTime(int arrivalTime);


    int getBurstTime();

    void setBurstTime(int burstTime);

    int getRemainingTime();

    void setRemainingTime(int remainingTime);

    int getTurnaroundTime();

    void setTurnaroundTime(int turnaroundTime);

    int getWaitingTime();

    void setWaitingTime(int waitingTime);

    void decrementRemainingTime();



    int getStartTime() const;
    void setStartTime(int newStartTime);

    int getFinishTime() const;
    void setFinishTime(int newFinishTime);

private:
    int processNumber;
    int arrivalTime;
    int priority;
    bool isPriority;
    int burstTime;
    int remainingTime;
    int turnaroundTime;
    int waitingTime;
    int startTime;
    int finishTime;
};

#endif // PROCESS_H
/**
 *

 *
 *
 *
*/

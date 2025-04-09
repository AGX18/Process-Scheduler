#ifndef PROCESS_H
#define PROCESS_H

class Process
{
public:
    Process(int processNumber, int arrivalTime, int burstTime);

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

private:
    int processNumber;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int turnaroundTime;
    int waitingTime;
};

#endif // PROCESS_H
/**
 *

 *
 *
 *
*/

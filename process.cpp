#include "process.h"

Process::Process(int processNumber, int arrivalTime, int burstTime, int priority) : processNumber(processNumber),
    arrivalTime(arrivalTime), burstTime(burstTime), remainingTime(burstTime), priority(priority), startTime(-1)
{

}



int Process::getProcessNumber() {
    return processNumber;
}


int Process::getArrivalTime() const {
    return arrivalTime;
}

void Process::setArrivalTime(int arrivalTime) {
    this->arrivalTime= arrivalTime;
}


int Process::getBurstTime() const {
    return burstTime;
}

void Process::setBurstTime(int burstTime) {
    this->burstTime = burstTime;
    this->remainingTime = burstTime;
}

int Process::getRemainingTime() const {
    return remainingTime;
}

void Process::setRemainingTime(int remainingTime) {
    this->remainingTime = remainingTime;
}

int Process::getTurnaroundTime() const {
    return turnaroundTime;
}

void Process::setTurnaroundTime(int turnaroundTime) {
    this->turnaroundTime = turnaroundTime;
}

int Process::getWaitingTime() const {
    return waitingTime;
}

void Process::setWaitingTime(int waitingTime) {
    this->waitingTime = waitingTime;
}
void Process::decrementRemainingTime() {
    if(remainingTime > 0){
        this->remainingTime--;
    }

}

int Process::getStartTime() const
{
    return startTime;
}

void Process::setStartTime(int newStartTime)
{
    startTime = newStartTime;
}

int Process::getFinishTime() const
{
    return finishTime;
}

void Process::setFinishTime(int newFinishTime)
{
    finishTime = newFinishTime;
}

int Process::getPriority() const
{
    return priority;
}

void Process::setPriority(int newPriority)
{
    priority = newPriority;
}

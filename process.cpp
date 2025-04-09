#include "process.h"

Process::Process(int processNumber, int arrivalTime, int burstTime) : processNumber(processNumber),
    arrivalTime(arrivalTime), burstTime(burstTime), remainingTime(burstTime)
{

}



int Process::getProcessNumber() {
    return processNumber;
}


int Process::getArrivalTime() {
    return arrivalTime;
}

void Process::setArrivalTime(int arrivalTime) {
    this->arrivalTime= arrivalTime;
}


int Process::getBurstTime() {
    return burstTime;
}

void Process::setBurstTime(int burstTime) {
    this->burstTime = burstTime;
    this->remainingTime = burstTime;
}

int Process::getRemainingTime() {
    return remainingTime;
}

void Process::setRemainingTime(int remainingTime) {
    this->remainingTime = remainingTime;
}

int Process::getTurnaroundTime() {
    return turnaroundTime;
}

void Process::setTurnaroundTime(int turnaroundTime) {
    this->turnaroundTime = turnaroundTime;
}

int Process::getWaitingTime() {
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

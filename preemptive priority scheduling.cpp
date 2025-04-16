#include <iostream>
#include <vector>
#include "process.h"
#include "scheduler.h"
using namespace std;

    int n;

    vector<Process*> processes;

    for (int i = 0; i < n; ++i) {
        int arrival, burst, priority;
        cin >> arrival >> burst >> priority;
        processes.push_back(new Process(i + 1, arrival, burst, priority));
    }

    int completed = 0, time = 0;
    float total_waiting_time = 0, total_turnaround_time = 0;

    while (completed < n) {
        int idx = -1;
        int min_priority = 1e9;

        for (int i = 0; i < n; ++i) {
            if (processes[i]->getRemainingTime() > 0 &&
                processes[i]->getArrivalTime() <= time &&
                processes[i]->priority < min_priority) {

                min_priority = processes[i]->priority;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        processes[idx]->decrementRemainingTime();
        time++;

        if (processes[idx]->getRemainingTime() == 0) {
            processes[idx]->setFinishTime(time);

            int turnaround_time = time - processes[idx]->getArrivalTime();
            int waiting_time = turnaround_time - processes[idx]->getBurstTime();

            processes[idx]->setTurnaroundTime(turnaround_time);
            processes[idx]->setWaitingTime(waiting_time);

            total_turnaround_time += turnaround_time;
            total_waiting_time += waiting_time;

            completed++;
        }
    }

    float avg_tat = total_turnaround_time / n;
    float avg_wt = total_waiting_time / n;

    cout << "\nAverage Turnaround Time: " << avg_tat << endl;
    cout << "Average Waiting Time: " << avg_wt << endl;

    
    for (auto p : processes) delete p;

    return 0;


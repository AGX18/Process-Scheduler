#include "PreemptivePriorityScheduling.h"
#include "process.h"
#include "mainwindow.h"
#include <iostream>
#include <vector>
#include <algorithm>

PreemptivePriorityScheduling::PreemptivePriorityScheduling(int n) : n(n), completed(0), time(0), total_waiting_time(0), total_turnaround_time(0) {
    processes.resize(n);
}

PreemptivePriorityScheduling::~PreemptivePriorityScheduling() {
    for (Process* p : processes) {
        delete p;
    }
    processes.clear();
}

void PreemptivePriorityScheduling::run() {
    std::cout << "Running Preemptive Scheduling (Preemptive by Earliest Arrival Time)...\n";

    for (int i = 0; i < n; ++i) {
        int arrival, burst;
        std::cout << "Enter arrival time and burst time for process " << i + 1 << ": ";
        std::cin >> arrival >> burst;
        processes[i] = new Process(i + 1, arrival, burst, 0); // أولوية افتراضية مش هتستخدم للاختيار
    }

    int currently_running = -1; // فهرس العملية اللي شغالة حاليًا (-1 لو مفيش)

    while (completed < n) {
        int next_process = -1;
        int earliest_arrival = 1e9;

        for (int i = 0; i < n; ++i) {
            if (processes[i]->getArrivalTime() <= time && processes[i]->getRemainingTime() > 0) {
                if (processes[i]->getArrivalTime() < earliest_arrival) {
                    earliest_arrival = processes[i]->getArrivalTime();
                    next_process = i;
                }
            }
        }

        if (next_process != -1) {
            if (next_process != currently_running) {
                if (currently_running != -1 && processes[next_process]->getArrivalTime() < processes[currently_running]->getArrivalTime()) {
                    // مقاطعة
                }
                currently_running = next_process;
            }

            processes[currently_running]->decrementRemainingTime();
            time++;

            if (processes[currently_running]->getRemainingTime() == 0) {
                completed++;
                processes[currently_running]->setFinishTime(time);
                processes[currently_running]->setTurnaroundTime(processes[currently_running]->getFinishTime() - processes[currently_running]->getArrivalTime());
                processes[currently_running]->setWaitingTime(processes[currently_running]->getTurnaroundTime() - processes[currently_running]->getBurstTime());
                currently_running = -1;
            }
        } else {
            time++;
        }
    }

    float avg_tat = total_turnaround_time / n;
    float avg_wt = total_waiting_time / n;

    std::cout << "\n--- Results ---\n";
    std::cout << "Average Turnaround Time: " << avg_tat << std::endl;
    std::cout << "Average Waiting Time: " << avg_wt << std::endl;
}

#include "fcfs.h"

FCFS::FCFS(QObject *parent = nullptr, std::vector<Process> processes = std::vector<Process>()) : Scheduler(parent, processes) {}


void FCFS::schedule() {
    if (processes.empty()) {
        return;
    }

    // Sort by arrival time
    sort(processes.begin(), processes.end(),
         [](const Process& a, const Process& b) {
        return a.getArrivalTime() < b.getArrivalTime();
         });

    int current_time = 0;
    for (auto& p : processes) {
        if (p.arrival_time > current_time) {
            current_time = p.arrival_time;
        }

        p.setStartTime(current_time);
        setFinishTime( current_time + p.burst_time );
        schedule.emplace_back(p.id, p.finish_time);
        current_time = p.finish_time;
    }

    /**
     * select a process every second to allocate the cpu to
     * wait for a second
     * then send a signal datachanged `emit datachanged(process.id);`
     * if the process has finished then emit another signal `emit processFinished(process.id,
     * process.waitingTime, process.TurnaroundTime);`
     */


}


/**
#include "pre_req.hpp"

vector<pair<int, int>> fcfs_scheduler(vector<Process>& processes) {
    vector<pair<int, int>> schedule;

    if (processes.empty()) {
        return schedule;
    }

    // Sort by arrival time
    sort(processes.begin(), processes.end(),
        [](const Process& a, const Process& b) {
            return a.arrival_time < b.arrival_time;
        });

    int current_time = 0;
    for (auto& p : processes) {
        if (p.arrival_time > current_time) {
            current_time = p.arrival_time;
        }

        p.start_time = current_time;
        p.finish_time = current_time + p.burst_time;
        schedule.emplace_back(p.id, p.finish_time);
        current_time = p.finish_time;
    }

    // Calculate and display results
    float total_waiting = 0, total_turnaround = 0;
    for (const auto& p : processes) {
        int waiting_time = p.start_time - p.arrival_time;
        int turnaround_time = p.finish_time - p.arrival_time;
        total_waiting += waiting_time;
        total_turnaround += turnaround_time;
    }

    cout << fixed << setprecision(2); // specifies that two digits should be shown after the decimal point.

    cout << "Average turnaround time: " << total_turnaround / processes.size() << endl;
    cout << "Average waiting time: " << total_waiting / processes.size() << endl;

    // Print schedule
    for (const auto& entry : schedule) {
        cout << "Process " << entry.first << " finishes at " << entry.second << endl;
    }

    return schedule;
}
*/

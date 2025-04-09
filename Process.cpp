#include <iostream>
#include<vector>
#include <deque>
#include <thread>
#include <mutex>
using namespace std;

int globaltime;
mutex time_mutex;
int current_time;
vector<bool> added;

struct Process {
    int ID;
    int arrival_time;
    int burst_time;
    int remaining_time;
    Process(int id, int arrival, int burst) : ID(id), arrival_time(arrival), burst_time(burst), remaining_time(burst) {}
};

deque<Process> ready;


void checkArrival(vector<Process>& processes) {

    for (int i = 0; i < processes.size(); i++)
        if (processes[i].arrival_time <= current_time && !added[i]) {
            ready.push_back(processes[i]);
            added[i] = true;
        }
}
void stopwatch(bool live_mode) {
    while (live_mode) {
        cout << "Time: " << current_time << " seconds" << endl;
        current_time++;
        this_thread::sleep_for(chrono::seconds(1));
    }
}


void ProcessInputThread(vector<Process>& processes, bool live_mode, int Q) {
    while (live_mode) {
        int id, burst_time;
        cout << "To add new process , Enter ID & Burst time : \n";
        cin >> id >> burst_time;
        Process p(id, current_time, burst_time);
        processes.push_back(p);
        added.push_back(false);
        checkArrival( processes);

        cout << "New Process P" << id << " added with Arrival Time: " << current_time
             << " and Burst Time: " << burst_time << endl;
    }
}



void RoundRobin(vector<Process>& processes, bool livemode, int Q) {



    int completed = 0;
    int turnaround_time = 0;
    int avg_turnaround_time = 0;
    float waiting_time = 0;
    float avg_waiting_time = 0;



    while (completed < processes.size() || livemode) {

        checkArrival( processes);

        if (ready.empty()) {
            current_time++;
            if (livemode)
                this_thread::sleep_for(chrono::seconds(1));
            continue;
        }


        Process current_process = ready.front();

        if (current_process.remaining_time <= Q) {

            if (livemode)  this_thread::sleep_for(chrono::seconds(current_process.remaining_time));
            else current_time += current_process.remaining_time;
            checkArrival(processes);
            ready.pop_front();
            turnaround_time = current_time - current_process.arrival_time;
            waiting_time = turnaround_time - current_process.burst_time;
            avg_turnaround_time += turnaround_time;
            avg_waiting_time += waiting_time;
            cout << "P" << current_process.ID << " terminated at completion time = " << current_time;
            cout << "  Waiting Time = " << waiting_time << "  Turnaround time = " << turnaround_time << endl;

            completed++;
        }
        else if ((current_process.remaining_time > Q)) {

            if (livemode)  this_thread::sleep_for(chrono::seconds(Q));
            else current_time += Q;

            checkArrival(processes);
            current_process.remaining_time -= Q;
            ready.pop_front();
            if (current_process.remaining_time == 0) {
                turnaround_time = current_time - current_process.arrival_time;
                waiting_time = turnaround_time - current_process.burst_time;
                avg_turnaround_time += turnaround_time;
                avg_waiting_time += waiting_time;
                cout << "P" << current_process.ID << " terminated at completion time = " << current_time << endl;
                cout << "  Waiting Time = " << waiting_time << "  Turnaround time = " << turnaround_time << endl;

                completed++;
            }
            else {
                ready.push_back(current_process);
            }

        }

    }
    cout << "Average turnaround time = " << avg_turnaround_time / processes.size() << endl;
    cout << "Average waiting time = " << avg_waiting_time / processes.size();

}


// int main()
// {


//     vector<Process> processes = {
//         Process(1,0,5),
//         Process(2,1,6),
//         Process(3,2,3),
//         Process(4,3,1),
//         Process(5,4,5),
//         Process(6,6,4)

//     };
//     added.resize(processes.size(), false);

//     thread watch, inputThread;
//     int Q = 4;
//     int live_mode = 1;
//     if (live_mode) {
//         watch = thread(stopwatch, live_mode);
//         inputThread = thread(ProcessInputThread, ref(processes), live_mode, Q);

//     }

//     RoundRobin(processes, live_mode, Q);

//     if (live_mode) {
//         watch.join();
//         inputThread.join();

//     }



// }

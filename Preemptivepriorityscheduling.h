#ifndef PREEMPTIVEPRIORITYSCHEDULING_H
#define PREEMPTIVEPRIORITYSCHEDULING_H

#include "process.h"
#include "scheduler.h"
#include <vector>

class PreemptivePriorityScheduling {
public:
    PreemptivePriorityScheduling(int n); // Constructor
    ~PreemptivePriorityScheduling();    // Destructor (لو كنتِ بتحجزي ذاكرة ديناميكية)

    void run();

private:
    int n;
    std::vector<Process*> processes; // استخدام std:: عشان لو مش عاملة using namespace std;
    int completed;
    int time;
    float total_waiting_time;
    float total_turnaround_time;

    // ممكن تضيفي هنا أي دوال مساعدة خاصة بالخوارزمية دي
    Process* getHighestPriorityProcess(); // مثال لدالة مساعدة
};
#endif // PREEMPTIVEPRIORITYSCHEDULING_H

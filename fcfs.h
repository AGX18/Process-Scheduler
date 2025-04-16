#ifndef FCFS_H
#define FCFS_H
#include "scheduler.h"


class FCFS : public scheduler
{
public:
    explicit FCFS(QObject *parent = nullptr, std::vector<Process> processes = std::vector<Process>());
};

#endif // FCFS_H





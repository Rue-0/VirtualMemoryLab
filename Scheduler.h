#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include <map>
#include <queue>
#include <vector>

enum class SchedulingPolicy {
    FCFS,
    RoundRobin,
    Priority,
    MLFQ
};

class Scheduler {
private:
    SchedulingPolicy policy;
    std::map<int, Process> processes;
    std::vector<std::queue<int>> readyQueues;
    std::map<int, int> processLevels;
    std::map<int, int> waitTimes;
    int quantum;
    int contextSwitches;
    int threadSwitches;
    int currentPid;
    int currentThread;

public:
    Scheduler(SchedulingPolicy policy, int tlbSize = 4);

    void addProcessIfNotExists(int pid);
    Process& getProcess(int pid);

    void addProcess(int pid, int priority);
    void addThread(int pid, int threadId, int priority);

    void tick();
    void adjustPriority(int pid, int threadId);
    void run();
    void printStats() const;

    int getContextSwitches() const;
    int getThreadSwitches() const;
};

#endif

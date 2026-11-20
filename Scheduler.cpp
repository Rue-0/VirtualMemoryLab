#include "Scheduler.h"
#include <iostream>
#include <algorithm>

Scheduler::Scheduler(SchedulingPolicy policy, int)
    : policy(policy),
      readyQueues(3),
      quantum(4),
      contextSwitches(0),
      threadSwitches(0),
      currentPid(-1),
      currentThread(-1) {}

void Scheduler::addProcessIfNotExists(int pid) {
    if (processes.find(pid) == processes.end()) {
        processes.emplace(pid, Process(pid, 0));
        readyQueues[0].push(pid);
        processLevels[pid] = 0;
        waitTimes[pid] = 0;
    }
}

Process& Scheduler::getProcess(int pid) {
    return processes.at(pid);
}

void Scheduler::addProcess(int pid, int priority) {
    if (processes.find(pid) == processes.end()) {
        processes.emplace(pid, Process(pid, priority));
        readyQueues[0].push(pid);
        processLevels[pid] = 0;
        waitTimes[pid] = 0;
    }
}

void Scheduler::addThread(int pid, int threadId, int priority) {
    addProcessIfNotExists(pid);
    processes.at(pid).addThread(threadId, priority);
}

void Scheduler::adjustPriority(int pid, int) {
    if (policy != SchedulingPolicy::MLFQ)
        return;

    int& level = processLevels[pid];

    if (level < 2)
        level++;
}

void Scheduler::tick() {
    // Aging: promote processes that wait too long.
    if (policy == SchedulingPolicy::MLFQ) {
        for (auto& entry : waitTimes) {
            entry.second++;

            if (entry.second >= 8 && processLevels[entry.first] > 0) {
                processLevels[entry.first]--;
                entry.second = 0;
            }
        }
    }
}

void Scheduler::run() {
    std::vector<int> order;

    for (const auto& entry : processes)
        order.push_back(entry.first);

    if (policy == SchedulingPolicy::Priority) {
        std::sort(order.begin(), order.end(),
            [this](int a, int b) {
                return processes.at(a).priority <
                       processes.at(b).priority;
            });
    }

    int previousPid = -1;

    for (int pid : order) {
        Process& process = processes.at(pid);

        if (previousPid != -1 && previousPid != pid)
            contextSwitches++;

        previousPid = pid;

        int previousThread = -1;

        for (const auto& thread : process.threadPriorities) {
            int threadId = thread.first;

            if (previousThread != -1 &&
                previousThread != threadId)
                threadSwitches++;

            previousThread = threadId;

            int cycles = quantum;

            if (policy == SchedulingPolicy::FCFS)
                cycles = std::max(1, process.cpuBurstRemaining);

            for (int i = 0; i < cycles; i++) {
                process.tick(threadId);
                tick();

                if (process.cpuBurstRemaining <= 0)
                    break;
            }

            if (policy == SchedulingPolicy::MLFQ)
                adjustPriority(pid, threadId);
        }
    }
}

void Scheduler::printStats() const {
    std::cout << "\n--- Scheduler Statistics ---\n";
    std::cout << "Context switches: "
              << contextSwitches << "\n";
    std::cout << "Thread switches: "
              << threadSwitches << "\n";

    if (policy == SchedulingPolicy::MLFQ)
        std::cout << "MLFQ queues: 3\n";
}

int Scheduler::getContextSwitches() const {
    return contextSwitches;
}

int Scheduler::getThreadSwitches() const {
    return threadSwitches;
}

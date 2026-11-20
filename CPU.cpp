#include "CPU.h"
#include <iostream>

CPU::CPU()
    : cyclesExecuted(0),
      contextSwitches(0),
      threadSwitches(0),
      lastPid(-1),
      lastThreadId(-1) {}

void CPU::execute(Process& process, int threadId) {
    if (lastPid != -1 && lastPid != process.pid)
        contextSwitches++;

    if (lastThreadId != -1 &&
        (lastThreadId != threadId || lastPid != process.pid))
        threadSwitches++;

    lastPid = process.pid;
    lastThreadId = threadId;

    process.tick(threadId);
    cyclesExecuted++;

    std::cout << "CPU: PID " << process.pid
              << " Thread " << threadId
              << " executed\n";
}

void CPU::printStats() const {
    std::cout << "\n--- CPU Statistics ---\n";
    std::cout << "Cycles executed: " << cyclesExecuted << "\n";
    std::cout << "Context switches: " << contextSwitches << "\n";
    std::cout << "Thread switches: " << threadSwitches << "\n";
}

int CPU::getCyclesExecuted() const {
    return cyclesExecuted;
}

int CPU::getContextSwitches() const {
    return contextSwitches;
}

int CPU::getThreadSwitches() const {
    return threadSwitches;
}

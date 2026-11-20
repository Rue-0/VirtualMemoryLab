#include "Process.h"

Process::Process(int pid, int priority)
    : pid(pid), priority(priority), cpuBurstRemaining(0), accessIndex(0) {}

bool Process::hasMoreAccesses() const {
    return accessIndex < memoryAccesses.size();
}

std::pair<int, unsigned int> Process::nextAccess() {
    if (!hasMoreAccesses()) {
        return {-1, 0};
    }
    return memoryAccesses[accessIndex++];
}

void Process::tick(int threadId) {
    if (cpuBurstRemaining > 0) {
        cpuBurstRemaining--;
    }

    if (threadPriorities.find(threadId) == threadPriorities.end()) {
        threadPriorities[threadId] = priority;
    }
}

void Process::addThread(int threadId, int threadPriority) {
    threadPriorities[threadId] = threadPriority;
}

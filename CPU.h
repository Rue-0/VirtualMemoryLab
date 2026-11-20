#ifndef CPU_H
#define CPU_H

#include "Process.h"

class CPU {
private:
    int cyclesExecuted;
    int contextSwitches;
    int threadSwitches;
    int lastPid;
    int lastThreadId;

public:
    CPU();

    void execute(Process& process, int threadId);
    void printStats() const;

    int getCyclesExecuted() const;
    int getContextSwitches() const;
    int getThreadSwitches() const;
};

#endif

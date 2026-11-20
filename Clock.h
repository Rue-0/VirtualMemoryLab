#ifndef CLOCK_H
#define CLOCK_H

#include <vector>
#include <cstddef>

struct FrameEntry {
    int pid;
    int pageNumber;
    bool referenced;
    bool modified;
    bool occupied;
};

class Clock {
private:
    std::vector<FrameEntry> frames;
    size_t clockHand;

public:
    Clock(int frameCount);

    int evictAndReplace(int pid, int pageNum,
                    int& evictedPid, int& evictedPage,
                    bool isModified);

    void referenceFrame(int frameNumber, bool isModified);
    void printStats() const;
};

#endif

#include "Clock.h"
#include <iostream>

Clock::Clock(int frameCount)
    : frames(frameCount), clockHand(0) {
    for (auto& frame : frames) {
        frame.pid = -1;
        frame.pageNumber = -1;
        frame.referenced = false;
        frame.modified = false;
        frame.occupied = false;
    }
}

int Clock::evictAndReplace(int pid, int pageNum,
                           int& evictedPid, int& evictedPage,
                           bool isModified) {
    // First use an empty frame if one exists.
    for (size_t i = 0; i < frames.size(); i++) {
        if (!frames[i].occupied) {
            frames[i] = {pid, pageNum, true, isModified, true};
            evictedPid = -1;
            evictedPage = -1;
            clockHand = (i + 1) % frames.size();
            return static_cast<int>(i);
        }
    }

    // Advanced Clock:
    // Prefer (referenced=0, modified=0) over dirty pages.
    while (true) {
        for (size_t i = 0; i < frames.size(); i++) {
            size_t index = (clockHand + i) % frames.size();

            if (!frames[index].referenced &&
                !frames[index].modified) {
                evictedPid = frames[index].pid;
                evictedPage = frames[index].pageNumber;
                frames[index] =
                    {pid, pageNum, true, isModified, true};

                clockHand = (index + 1) % frames.size();
                return static_cast<int>(index);
            }
        }

        // Give referenced pages a second chance.
        for (auto& frame : frames) {
            if (frame.referenced)
                frame.referenced = false;
        }

        // If no clean page exists, replace an
        // unreferenced modified page.
        for (size_t i = 0; i < frames.size(); i++) {
            size_t index = (clockHand + i) % frames.size();

            if (!frames[index].referenced) {
                evictedPid = frames[index].pid;
                evictedPage = frames[index].pageNumber;
                frames[index] =
                    {pid, pageNum, true, isModified, true};

                clockHand = (index + 1) % frames.size();
                return static_cast<int>(index);
            }
        }
    }
}

void Clock::referenceFrame(int frameNumber, bool isModified) {
    if (frameNumber < 0 ||
        frameNumber >= static_cast<int>(frames.size()))
        return;

    frames[frameNumber].referenced = true;

    if (isModified)
        frames[frameNumber].modified = true;
}

void Clock::printStats() const {
    std::cout << "\n--- Physical Frames ---\n";

    for (size_t i = 0; i < frames.size(); i++) {
        if (frames[i].occupied) {
            std::cout << "Frame " << i
                      << ": PID=" << frames[i].pid
                      << " Page=" << frames[i].pageNumber
                      << " R=" << frames[i].referenced
                      << " M=" << frames[i].modified
                      << "\n";
        }
    }
}

#include "TwoLevelPageTable.h"
#include <iostream>

TwoLevelPageTable::TwoLevelPageTable(int pageCount)
    : pageCount(pageCount) {}

bool TwoLevelPageTable::isInMemory(int pid, int pageNum) const {
    return memoryPages.count({pid, pageNum}) > 0;
}

int TwoLevelPageTable::getFrame(int pid, int pageNum) const {
    auto it = secondLevel.find({pid, pageNum});

    if (it == secondLevel.end())
        return -1;

    return it->second;
}

void TwoLevelPageTable::loadPage(int pid, int pageNum, int frameNum) {
    int outerPage = pageNum / 16;
    int innerPage = pageNum % 16;

    firstLevel[{pid, outerPage}] = outerPage;
    secondLevel[{pid, pageNum}] = frameNum;
    memoryPages.insert({pid, pageNum});

    std::cout << "Page loaded: PID " << pid
              << " outer=" << outerPage
              << " inner=" << innerPage
              << " frame=" << frameNum << "\n";
}

void TwoLevelPageTable::removePage(int pid, int pageNum) {
    secondLevel.erase({pid, pageNum});
    memoryPages.erase({pid, pageNum});
}

void TwoLevelPageTable::printStats() const {
    std::cout << "\n--- Two-Level Page Table ---\n";
    std::cout << "Pages currently in memory: "
              << memoryPages.size() << "\n";
    std::cout << "Configured virtual pages: "
              << pageCount << "\n";
}

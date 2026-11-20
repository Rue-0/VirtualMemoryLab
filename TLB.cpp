#include "TLB.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

TLB::TLB(int size)
    : currentTime(0), maxSize(size), hits(0), misses(0) {}

bool TLB::lookup(int pid, int pageNumber, int& frameNumber) {
    currentTime++;

    for (auto& entry : tlbEntries) {
        if (entry.pid == pid && entry.pageNumber == pageNumber) {
            frameNumber = entry.frameNumber;
            entry.lastUsed = currentTime;
            hits++;
            return true;
        }
    }

    misses++;
    return false;
}

void TLB::insert(int pid, int pageNumber, int frameNumber) {
    currentTime++;

    for (auto& entry : tlbEntries) {
        if (entry.pid == pid && entry.pageNumber == pageNumber) {
            entry.frameNumber = frameNumber;
            entry.lastUsed = currentTime;
            return;
        }
    }

    if ((int)tlbEntries.size() >= maxSize) {
        auto lru = std::min_element(
            tlbEntries.begin(),
            tlbEntries.end(),
            [](const TLBEntry& a, const TLBEntry& b) {
                return a.lastUsed < b.lastUsed;
            });

        tlbEntries.erase(lru);
    }

    tlbEntries.push_back({pid, pageNumber, frameNumber, currentTime});
}

void TLB::invalidate(int pid, int pageNumber) {
    tlbEntries.erase(
        std::remove_if(
            tlbEntries.begin(),
            tlbEntries.end(),
            [pid, pageNumber](const TLBEntry& entry) {
                return entry.pid == pid &&
                       entry.pageNumber == pageNumber;
            }),
        tlbEntries.end());
}

int TLB::getHits() const {
    return hits;
}

int TLB::getMisses() const {
    return misses;
}

double TLB::getHitRate() const {
    int total = hits + misses;

    if (total == 0)
        return 0.0;

    return (100.0 * hits) / total;
}

void TLB::printStats() const {
    std::cout << "\n--- TLB Statistics ---\n";
    std::cout << "TLB Size: " << maxSize << "\n";
    std::cout << "TLB Hits: " << hits << "\n";
    std::cout << "TLB Misses: " << misses << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "TLB Hit Rate: " << getHitRate() << "%\n";
}

#include "Process.h"
#include "CPU.h"
#include "TLB.h"
#include "TwoLevelPageTable.h"
#include "Clock.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

int main(int argc, char* argv[]) {
    int tlbSize = 4;

    if (argc > 1)
        tlbSize = std::stoi(argv[1]);

    const int frameCount = 4;
    const unsigned int pageSize = 256;

    TLB tlb(tlbSize);
    TwoLevelPageTable pageTable(256);
    Clock clock(frameCount);
    CPU cpu;

    std::map<int, Process> processes;

    int totalAccesses = 0;
    int pageFaults = 0;

    std::ifstream input("input/trace.txt");
    std::ofstream log("output.log");

    if (!input) {
        std::cerr << "Error: could not open input/trace.txt\n";
        return 1;
    }

    std::string line;

    std::cout << "Virtual Memory + CPU Simulator\n";
    std::cout << "TLB Size: " << tlbSize << "\n\n";

    while (std::getline(input, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::stringstream ss(line);

        int pid;
        int threadId;
        std::string addressText;
        char accessType;

        ss >> pid >> threadId >> addressText >> accessType;

        unsigned int address =
            static_cast<unsigned int>(
                std::stoul(addressText, nullptr, 16));

        int pageNumber = address / pageSize;
        bool modified = (accessType == 'W');

        if (processes.find(pid) == processes.end())
            processes.emplace(pid, Process(pid, pid));

        Process& process = processes.at(pid);
        process.addThread(threadId, pid);
        process.cpuBurstRemaining++;

        totalAccesses++;

        int frame = -1;
        bool tlbHit = tlb.lookup(pid, pageNumber, frame);

        if (tlbHit) {
            clock.referenceFrame(frame, modified);

            log << "PID " << pid
                << " Thread " << threadId
                << " Page " << pageNumber
                << " TLB HIT"
                << " Frame " << frame << "\n";
        } else {
            if (!pageTable.isInMemory(pid, pageNumber)) {
                pageFaults++;
                int evictedPid = -1;
                int evictedPage = -1;

                frame = clock.evictAndReplace(
               pid,
               pageNumber,
                  evictedPid,
                  evictedPage,
                modified
                                                );
                if (evictedPid != -1 && evictedPage != -1) {
                pageTable.removePage(evictedPid, evictedPage);
                tlb.invalidate(evictedPid, evictedPage);
                }

                pageTable.loadPage(pid, pageNumber, frame);

                log << "PID " << pid
                    << " Thread " << threadId
                    << " Page " << pageNumber
                    << " PAGE FAULT"
                    << " Frame " << frame << "\n";
            } else {
                frame = pageTable.getFrame(pid, pageNumber);

                log << "PID " << pid
                    << " Thread " << threadId
                    << " Page " << pageNumber
                    << " TLB MISS"
                    << " Frame " << frame << "\n";
            }

            tlb.insert(pid, pageNumber, frame);
        }

        cpu.execute(process, threadId);

        std::cout << "PID " << pid
                  << " Thread " << threadId
                  << " Address 0x" << std::hex << address
                  << std::dec
                  << " Page " << pageNumber
                  << (tlbHit ? " [TLB HIT]" : " [TLB MISS]")
                  << "\n";
    }

    std::cout << "\n===== PERFORMANCE SUMMARY =====\n";

    tlb.printStats();

    std::cout << "Total memory accesses: "
              << totalAccesses << "\n";

    std::cout << "Page faults: "
              << pageFaults << "\n";

    double faultRate =
        totalAccesses == 0 ? 0.0 :
        (100.0 * pageFaults / totalAccesses);

    std::cout << "Page Fault Rate: "
              << faultRate << "%\n";

    cpu.printStats();
    pageTable.printStats();
    clock.printStats();

    std::cout << "\nOutput log written to output.log\n";

    return 0;
}

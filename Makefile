CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

TARGET = simulator

SOURCES = main.cpp \
          Process.cpp \
          CPU.cpp \
          Scheduler.cpp \
          TLB.cpp \
          TwoLevelPageTable.cpp \
          Clock.cpp

HEADERS = Process.h \
          CPU.h \
          Scheduler.h \
          TLB.h \
          TwoLevelPageTable.h \
          Clock.h

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) 4

clean:
	rm -f $(TARGET) output.log

.PHONY: run clean

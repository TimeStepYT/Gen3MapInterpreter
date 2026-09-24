#pragma once

#include <chrono>

class Profiler {
private:
    std::chrono::high_resolution_clock::time_point m_startTime = std::chrono::high_resolution_clock::now();

public:
    Profiler();

    long long getTime();
    double getTimeSeconds();
    double getTimeMilliseconds();
};
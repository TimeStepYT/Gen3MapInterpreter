#pragma once

#include <chrono>

class Profiler {
private:
    std::chrono::high_resolution_clock::time_point m_startTime = std::chrono::high_resolution_clock::now();

public:
    Profiler();

    long long getNanoseconds();
    double getMicroseconds();
    double getMilliseconds();
    double getSeconds();
    void printNanoseconds(std::string_view label = "");
    void printMicroseconds(std::string_view label = "");
    void printMilliseconds(std::string_view label = "");
    void printSeconds(std::string_view label = "");
};
#include <Profiler.hpp>

Profiler::Profiler() {
    
}

long long Profiler::getTime() {
    auto now = std::chrono::high_resolution_clock::now();
    auto diff = now - this->m_startTime;
    return diff.count();
}

double Profiler::getTimeSeconds() {
    double nano = this->getTime();

    return nano / 1000000000.f;
}

double Profiler::getTimeMilliseconds() {
    double nano = this->getTime();

    return nano / 1000000.f;
}
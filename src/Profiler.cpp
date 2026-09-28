#include <Profiler.hpp>
#include <iostream>
#include <fmt/core.h>
#include <fmt/format.h>

Profiler::Profiler() {
    
}

long long Profiler::getNanoseconds() {
    auto now = std::chrono::high_resolution_clock::now();
    auto diff = now - this->m_startTime;
    return diff.count();
}

double Profiler::getSeconds() {
    double nano = this->getNanoseconds();

    return nano / 1000000000.f;
}

double Profiler::getMilliseconds() {
    double nano = this->getNanoseconds();

    return nano / 1000000.f;
}

double Profiler::getMicroseconds() {
    double nano = this->getNanoseconds();

    return nano / 1000.f;
}

void Profiler::printNanoseconds(std::string_view label) {
    fmt::println("{}{}ns", label.size() == 0 ? "" : fmt::format("{}: ", label), this->getNanoseconds());
}

void Profiler::printSeconds(std::string_view label) {
    fmt::println("{}{}s", label.size() == 0 ? "" : fmt::format("{}: ", label), this->getSeconds());
}

void Profiler::printMilliseconds(std::string_view label) {
    fmt::println("{}{}ms", label.size() == 0 ? "" : fmt::format("{}: ", label), this->getMilliseconds());
}

void Profiler::printMicroseconds(std::string_view label) {
    fmt::println("{}{}µs", label.size() == 0 ? "" : fmt::format("{}: ", label), this->getMicroseconds());
}
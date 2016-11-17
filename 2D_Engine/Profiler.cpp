#include "Profiler.h"

#include <chrono>
#include <iostream>

using namespace std::chrono;

Profiler::Profiler(const std::string& name)
{
    start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

Profiler::~Profiler()
{
    long long deltaTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - start;
    std::cout << (double)deltaTime / 1000.f << std::endl;
}

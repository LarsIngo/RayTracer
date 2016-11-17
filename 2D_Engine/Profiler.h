#pragma once

#include <string>

// Profiler.
class Profiler {
    public:
        // Constructor.
        Profiler(const std::string& name);

        // Destructor.
        ~Profiler();

    private:
        long long start;
};

#define PROFILE(name) Profiler __profileInstance(name)

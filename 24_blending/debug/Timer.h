#pragma once

#include <chrono>

namespace debug {

class Timer {
public:
    Timer();
    void Loop(const char* str);
private:
    std::chrono::time_point<std::chrono::steady_clock> m_start;
};

}

#include <iostream>
#include "Timer.h"

using namespace debug;

Timer::Timer() {
    m_start = std::chrono::high_resolution_clock::now();
}

void Timer::Loop(const char* str) {
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - m_start;
    
    std::cout << str << duration.count() << "s\n";
    
    m_start = end;
}

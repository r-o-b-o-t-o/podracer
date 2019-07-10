#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    Timer();
    void reset();
    float elapsed() const;

private:
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::duration<float, std::ratio<1>> second;
    std::chrono::time_point<clock> startTime;
};

#endif

#include "Timer.h"

Timer::Timer() :
        startTime(clock::now()) {

}

void Timer::reset() {
    this->startTime = clock::now();
}

float Timer::elapsed() const {
    return std::chrono::duration_cast<second>(clock::now() - this->startTime).count();
}

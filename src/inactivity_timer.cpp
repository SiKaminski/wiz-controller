#include "inactivity_timer.hpp"

#include <chrono>

InactivityTimer::InactivityTimer(std::chrono::milliseconds timeout) :
    mTimeout(timeout), mLast(std::chrono::steady_clock::now()) {}

void InactivityTimer::reset()
{
    mLast = std::chrono::steady_clock::now();
}

bool InactivityTimer::expired()
{
    return std::chrono::steady_clock::now() - mLast >= mTimeout;
}

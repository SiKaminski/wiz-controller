#pragma once
#ifndef _INACTIVITY_TIMER_HPP
#define _INACTIVITY_TIMER_HPP

#include <chrono>

class InactivityTimer
{
    public:
        InactivityTimer(std::chrono::milliseconds timeout);

        void reset();
        bool expired();

    private:
        std::chrono::milliseconds mTimeout;
        std::chrono::steady_clock::time_point mLast;
};

#endif // _INACTIVITY_TIMER_HPP


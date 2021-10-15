#pragma once

#include <chrono>

class Timer
{
    float m_interval;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTick;

  public:
    explicit Timer(float interval);
    bool Tick();
};

#pragma once

#include <chrono>

class Timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTick;
    float m_interval;

  public:
    explicit Timer(float interval);
    bool Tick();
};
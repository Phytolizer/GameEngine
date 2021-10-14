#include "Timer.hpp"
#include <chrono>

Timer::Timer(float interval)
    : m_interval(interval),
      m_lastTick(std::chrono::high_resolution_clock::now())
{
}
bool Timer::Tick()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::ratio<1, 1>> elapsed = now - m_lastTick;
    if (elapsed.count() > m_interval)
    {
        m_lastTick = now;
        return true;
    }

    return false;
}
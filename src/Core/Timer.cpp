#include "TinyEngine/Core/Timer.h"

namespace TinyEngine::Core {

Timer::Timer()
    : m_startTime(Clock::now()),
      m_previousTick(m_startTime),
      m_deltaSeconds(0.0) {}

void Timer::Reset() {
    m_startTime = Clock::now();
    m_previousTick = m_startTime;
    m_deltaSeconds = 0.0;
}

void Timer::Tick() {
    const Clock::time_point current = Clock::now();
    m_deltaSeconds = std::chrono::duration<double>(current - m_previousTick).count();
    m_previousTick = current;
}

double Timer::GetDeltaTimeSeconds() const {
    return m_deltaSeconds;
}

double Timer::GetElapsedTimeSeconds() const {
    const Clock::time_point current = Clock::now();
    return std::chrono::duration<double>(current - m_startTime).count();
}

} // namespace TinyEngine::Core

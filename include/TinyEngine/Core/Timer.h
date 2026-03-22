#pragma once

#include <chrono>

namespace TinyEngine::Core {

class Timer {
public:
    Timer();

    void Reset();
    void Tick();

    [[nodiscard]] double GetDeltaTimeSeconds() const;
    [[nodiscard]] double GetElapsedTimeSeconds() const;

private:
    using Clock = std::chrono::steady_clock;
    Clock::time_point m_startTime;
    Clock::time_point m_previousTick;
    double m_deltaSeconds = 0.0;
};

} // namespace TinyEngine::Core

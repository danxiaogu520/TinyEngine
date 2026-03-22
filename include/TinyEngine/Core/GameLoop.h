#pragma once

#include <functional>

#include "TinyEngine/Core/Event.h"

namespace TinyEngine::Core {

class Window;

struct GameLoopCallbacks {
    std::function<void()> onInit;
    std::function<void(const Event&)> onEvent;
    std::function<void(double)> onUpdate;
    std::function<void()> onShutdown;
};

class GameLoop {
public:
    bool Run(Window& window, const GameLoopCallbacks& callbacks);
};

} // namespace TinyEngine::Core

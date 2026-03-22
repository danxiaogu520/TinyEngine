#include "TinyEngine/Core/GameLoop.h"

#include "TinyEngine/Core/Timer.h"
#include "TinyEngine/Core/Window.h"

namespace TinyEngine::Core {
	bool GameLoop::Run(Window& window, const GameLoopCallbacks& callbacks) {
		if (!window.Initialize()) {
			return false;
		}

		if (callbacks.onInit) {
			callbacks.onInit();
		}

		Timer timer;
		timer.Reset();

		while (!window.ShouldClose()) {
			timer.Tick();

			Event event;
			while (window.PollEvent(event)) {
				if (callbacks.onEvent) {
					callbacks.onEvent(event);
				}
			}

			if (callbacks.onUpdate) {
				callbacks.onUpdate(timer.GetDeltaTimeSeconds());
			}
		}

		if (callbacks.onShutdown) {
			callbacks.onShutdown();
		}

		return true;
	}
} // namespace TinyEngine::Core

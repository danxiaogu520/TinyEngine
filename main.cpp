#include <iostream>

#include "TinyEngine/Core/Event.h"
#include "TinyEngine/Core/GameLoop.h"
#include "TinyEngine/Core/Window.h"

int main() {
	TinyEngine::Core::Window window({"胆小菇QAQ", 1280, 720});
	TinyEngine::Core::GameLoop loop;

	TinyEngine::Core::GameLoopCallbacks callbacks;
	callbacks.onInit = []() {
		std::cout << "Game loop started." << std::endl;
	};
	callbacks.onEvent = [](const TinyEngine::Core::Event& event) {
		if (event.type == TinyEngine::Core::EventType::WindowResized) {
			std::cout << "Window resized to: " << event.width << "x" << event.height << std::endl;
		}
	};
	callbacks.onUpdate = [](double deltaTimeSeconds) {
		static double x = 0.0;
		static double speed = 100.0;
		static double simulationAccumulator = 0.0;
		static double printAccumulator = 0.0;
		constexpr double fixedDeltaSeconds = 1.0 / 60.0;

		simulationAccumulator += deltaTimeSeconds;
		while (simulationAccumulator >= fixedDeltaSeconds) {
			x += speed * fixedDeltaSeconds;
			simulationAccumulator -= fixedDeltaSeconds;
		}

		printAccumulator += deltaTimeSeconds;

		while (printAccumulator >= 1.0) {
			std::cout << "position x: " << x << ", speed: " << speed << std::endl;
			printAccumulator -= 1.0;
		}
	};
	callbacks.onShutdown = []() {
		std::cout << "Game loop stopped." << std::endl;
	};

	if (!loop.Run(window, callbacks)) {
		std::cerr << "Failed to start game loop." << std::endl;
		return 1;
	}

	return 0;
}

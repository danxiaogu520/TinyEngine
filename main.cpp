#include <cmath>
#include <iostream>

#include "TinyEngine/Core/Event.h"
#include "TinyEngine/Core/GameLoop.h"
#include "TinyEngine/Core/Log.h"
#include "TinyEngine/Core/Window.h"
#include "TinyEngine/Graphics/Renderer2D.h"

int main() {
	TinyEngine::Core::Window window({"胆小菇QAQ", 1280, 720});
	TinyEngine::Core::GameLoop loop;
	TinyEngine::Graphics::Renderer2D renderer;

	TinyEngine::Core::GameLoopCallbacks callbacks;
	callbacks.onInit = [&]() {
		TinyEngine::Core::Log::Info("Game loop started.");
		if (!renderer.Initialize(window)) {
			TinyEngine::Core::Log::Error("Renderer initialization failed.");
			window.RequestClose();
			return;
		}

		renderer.SetClearColor(0.10f, 0.12f, 0.18f, 1.0f);
	};
	callbacks.onEvent = [&](const TinyEngine::Core::Event& event) {
		if (event.type == TinyEngine::Core::EventType::WindowResized) {
			std::cout << "Window resized to: " << event.width << "x" << event.height << std::endl;
			renderer.OnResize(event.width, event.height);
		}
	};
	callbacks.onUpdate = [&](const double deltaTimeSeconds) {
		static double x = 0.0;
		static double y = 0.0;
		static double speed = 100.0;
		static double simulationAccumulator = 0.0;
		static double printAccumulator = 0.0;
		constexpr int boxWidth = 100;
		constexpr int boxHeight = 100;
		constexpr int trackMinX = 0;
		constexpr int trackMaxX = 1180;
		constexpr double baseY = 320.0;
		constexpr double amplitude = 180.0;
		constexpr double cyclesAcrossScreen = 2.0;
		constexpr double fixedDeltaSeconds = 1.0 / 60.0;

		simulationAccumulator += deltaTimeSeconds;
		while (simulationAccumulator >= fixedDeltaSeconds) {
			x += speed * fixedDeltaSeconds;
			if (x >= trackMaxX) {
				x = static_cast<double>(trackMaxX);
				speed = -speed;
			}
			if (x <= trackMinX) {
				x = static_cast<double>(trackMinX);
				speed = -speed;
			}

			const double normalizedX = (x - static_cast<double>(trackMinX)) /
				(static_cast<double>(trackMaxX - trackMinX));
			const double phase = normalizedX * cyclesAcrossScreen * 2.0 * 3.141592653589793;
			y = baseY + amplitude * std::sin(phase);
			simulationAccumulator -= fixedDeltaSeconds;
		}

		renderer.SetClearColor(0.08f, 0.10f, 0.16f, 1.0f);
		renderer.BeginFrame();
		renderer.DrawFilledRect(static_cast<int>(x), static_cast<int>(y), boxWidth, boxHeight, 0.92f, 0.30f, 0.35f, 1.0f);
		renderer.EndFrame();

		printAccumulator += deltaTimeSeconds;

		while (printAccumulator >= 1.0) {
			std::cout << "position x: " << x << ", y: " << y << ", speed: " << speed << std::endl;
			printAccumulator -= 1.0;
		}
	};
	callbacks.onShutdown = [&]() {
		renderer.Shutdown();
		TinyEngine::Core::Log::Info("Game loop stopped.");
	};

	if (!loop.Run(window, callbacks)) {
		std::cerr << "Failed to start game loop." << std::endl;
		return 1;
	}

	return 0;
}

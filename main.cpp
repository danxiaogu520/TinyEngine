#include <iostream>

#include "TinyEngine/Core/Event.h"
#include "TinyEngine/Core/Window.h"

int main() {
	TinyEngine::Core::Window window({"胆小菇QAQ", 1280, 720});
	if (!window.Initialize()) {
		std::cerr << "Failed to initialize SDL2 window." << std::endl;
		return 1;
	}

	while (!window.ShouldClose()) {
		TinyEngine::Core::Event event;
		while (window.PollEvent(event)) {
			if (event.type == TinyEngine::Core::EventType::WindowResized) {
				std::cout << "Window resized to: " << event.width << "x" << event.height << std::endl;
			}
		}
	}

	return 0;
}
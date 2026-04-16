#pragma once

#include <cstdint>

namespace TinyEngine::Core {
	enum class EventType {
		None = 0,
		Quit,
		WindowResized,
		KeyDown,
		KeyUp,
		MouseButtonDown,
		MouseButtonUp,
		MouseMoved,
		MouseWheel
	};

	struct Event {
		EventType type = EventType::None;
		int width = 0;
		int height = 0;
		int keyCode = 0;
		bool isRepeat = false;
		std::uint8_t mouseButton = 0;
		int mouseX = 0;
		int mouseY = 0;
		int mouseDeltaX = 0;
		int mouseDeltaY = 0;
		int wheelX = 0;
		int wheelY = 0;
	};
} // namespace TinyEngine::Core

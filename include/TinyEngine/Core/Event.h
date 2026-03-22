#pragma once

namespace TinyEngine::Core {
	enum class EventType {
		None = 0,
		Quit,
		WindowResized
	};

	struct Event {
		EventType type = EventType::None;
		int width = 0;
		int height = 0;
	};
} // namespace TinyEngine::Core

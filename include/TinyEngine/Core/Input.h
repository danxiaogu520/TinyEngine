#pragma once

#include <cstdint>
#include <functional>
#include <unordered_set>

#include "TinyEngine/Core/Event.h"

namespace TinyEngine::Core {
	class Input {
	public:
		using KeyCallback = std::function<void(int keyCode)>;
		using MouseButtonCallback = std::function<void(std::uint8_t button, int x, int y)>;
		using MouseMoveCallback = std::function<void(int x, int y, int deltaX, int deltaY)>;

		static Input& Instance();

		void Reset();
		void ProcessEvent(const Event& event);

		[[nodiscard]] bool IsKeyDown(int keyCode) const;
		[[nodiscard]] bool IsMouseButtonDown(std::uint8_t button) const;
		[[nodiscard]] int MouseX() const;
		[[nodiscard]] int MouseY() const;

		void SetKeyDownCallback(KeyCallback callback);
		void SetKeyUpCallback(KeyCallback callback);
		void SetMouseButtonDownCallback(MouseButtonCallback callback);
		void SetMouseButtonUpCallback(MouseButtonCallback callback);
		void SetMouseMoveCallback(MouseMoveCallback callback);

	private:
		Input() = default;

		std::unordered_set<int> m_keysDown;
		std::unordered_set<std::uint8_t> m_mouseButtonsDown;
		int m_mouseX = 0;
		int m_mouseY = 0;

		KeyCallback m_onKeyDown;
		KeyCallback m_onKeyUp;
		MouseButtonCallback m_onMouseButtonDown;
		MouseButtonCallback m_onMouseButtonUp;
		MouseMoveCallback m_onMouseMove;
	};
} // namespace TinyEngine::Core
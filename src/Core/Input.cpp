#include "TinyEngine/Core/Input.h"

#include <utility>

namespace TinyEngine::Core {
	Input& Input::Instance() {
		static Input input;
		return input;
	}

	void Input::Reset() {
		m_keysDown.clear();
		m_mouseButtonsDown.clear();
		m_mouseX = 0;
		m_mouseY = 0;
	}

	void Input::ProcessEvent(const Event& event) {
		switch (event.type) {
		case EventType::KeyDown:
			m_keysDown.insert(event.keyCode);
			if (!event.isRepeat && m_onKeyDown) {
				m_onKeyDown(event.keyCode);
			}
			break;
		case EventType::KeyUp:
			m_keysDown.erase(event.keyCode);
			if (m_onKeyUp) {
				m_onKeyUp(event.keyCode);
			}
			break;
		case EventType::MouseButtonDown:
			m_mouseButtonsDown.insert(event.mouseButton);
			m_mouseX = event.mouseX;
			m_mouseY = event.mouseY;
			if (m_onMouseButtonDown) {
				m_onMouseButtonDown(event.mouseButton, event.mouseX, event.mouseY);
			}
			break;
		case EventType::MouseButtonUp:
			m_mouseButtonsDown.erase(event.mouseButton);
			m_mouseX = event.mouseX;
			m_mouseY = event.mouseY;
			if (m_onMouseButtonUp) {
				m_onMouseButtonUp(event.mouseButton, event.mouseX, event.mouseY);
			}
			break;
		case EventType::MouseMoved:
			m_mouseX = event.mouseX;
			m_mouseY = event.mouseY;
			if (m_onMouseMove) {
				m_onMouseMove(event.mouseX, event.mouseY, event.mouseDeltaX, event.mouseDeltaY);
			}
			break;
		default:
			break;
		}
	}

	bool Input::IsKeyDown(const int keyCode) const {
		return m_keysDown.find(keyCode) != m_keysDown.end();
	}

	bool Input::IsMouseButtonDown(const std::uint8_t button) const {
		return m_mouseButtonsDown.find(button) != m_mouseButtonsDown.end();
	}

	int Input::MouseX() const {
		return m_mouseX;
	}

	int Input::MouseY() const {
		return m_mouseY;
	}

	void Input::SetKeyDownCallback(KeyCallback callback) {
		m_onKeyDown = std::move(callback);
	}

	void Input::SetKeyUpCallback(KeyCallback callback) {
		m_onKeyUp = std::move(callback);
	}

	void Input::SetMouseButtonDownCallback(MouseButtonCallback callback) {
		m_onMouseButtonDown = std::move(callback);
	}

	void Input::SetMouseButtonUpCallback(MouseButtonCallback callback) {
		m_onMouseButtonUp = std::move(callback);
	}

	void Input::SetMouseMoveCallback(MouseMoveCallback callback) {
		m_onMouseMove = std::move(callback);
	}
} // namespace TinyEngine::Core
#pragma once

#include <string_view>

namespace TinyEngine::Core {
	enum class LogLevel {
		Info,
		Warning,
		Error
	};

	class Log {
	public:
		static void Message(LogLevel level, std::string_view message);
		static void Info(std::string_view message);
		static void Warning(std::string_view message);
		static void Error(std::string_view message);
	};
} // namespace TinyEngine::Core

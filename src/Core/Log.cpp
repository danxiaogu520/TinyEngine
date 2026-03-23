#include "TinyEngine/Core/Log.h"

#include <iostream>

namespace TinyEngine::Core {
	namespace {
		const char* ToLabel(LogLevel level) {
			switch (level) {
			case LogLevel::Info: return "INFO";
			case LogLevel::Warning: return "WARN";
			case LogLevel::Error: return "ERROR";
			default: return "UNKNOWN";
			}
		}
	} // namespace

	void Log::Message(LogLevel level, std::string_view message) {
		std::ostream& out = (level == LogLevel::Error) ? std::cerr : std::cout;
		out << "[" << ToLabel(level) << "] " << message << std::endl;
	}

	void Log::Info(std::string_view message) {
		Message(LogLevel::Info, message);
	}

	void Log::Warning(std::string_view message) {
		Message(LogLevel::Warning, message);
	}

	void Log::Error(std::string_view message) {
		Message(LogLevel::Error, message);
	}
} // namespace TinyEngine::Core

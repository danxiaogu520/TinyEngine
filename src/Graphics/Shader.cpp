#include "TinyEngine/Graphics/Shader.h"

#include <fstream>
#include <sstream>
#include <utility>

namespace TinyEngine::Graphics {
	namespace {
		std::string ReadTextFile(std::string_view path) {
			std::ifstream inFile{std::string (path)};
			if (!inFile.is_open()) {
				return {};
			}

			std::ostringstream oss;
			oss << inFile.rdbuf();
			return oss.str();
		}
	} // namespace

	Shader::~Shader() {
		Reset();
	}

	Shader::Shader(Shader&& other) noexcept : m_programId(other.m_programId),
											  m_lastError(std::move(other.m_lastError)) {
		other.m_programId = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept {
		if (this == &other) {
			return *this;
		}

		Reset();

		m_programId = other.m_programId;
		m_lastError = std::move(other.m_lastError);
		other.m_programId = 0;
		return *this;
	}

	bool Shader::LoadFromFiles(std::string_view vertexPath, std::string_view fragmentPath) {
		const std::string vertexSource = ReadTextFile(vertexPath);
		if (vertexSource.empty()) {
			m_lastError = "Failed to read vertex shader file.";
			return false;
		}

		const std::string fragmentSource = ReadTextFile(fragmentPath);
		if (fragmentSource.empty()) {
			m_lastError = "Failed to read fragment shader file.";
			return false;
		}

		return LoadFromSource(vertexSource, fragmentSource);
	}

	bool Shader::LoadFromSource(std::string_view vertexSource, std::string_view fragmentSource) {
		Reset();
		m_lastError.clear();

		if (vertexSource.empty()) {
			m_lastError = "Vertex shader source is empty.";
			return false;
		}

		if (fragmentSource.empty()) {
			m_lastError = "Fragment shader source is empty.";
			return false;
		}

		m_isLoaded = true;
		m_programId = 1;
		return true;
	}

	void Shader::Bind() const {
		// Day9 minimal path: no GPU-side bind yet.
	}

	void Shader::Unbind() const {
		// Day9 minimal path: no GPU-side unbind yet.
	}

	void Shader::Reset() {
		m_isLoaded = false;
		m_programId = 0;
	}

	bool Shader::IsValid() const {
		return m_isLoaded;
	}

	std::uint32_t Shader::GetProgramId() const {
		return m_programId;
	}

	const std::string& Shader::GetLastError() const {
		return m_lastError;
	}
} // namespace TinyEngine::Graphics

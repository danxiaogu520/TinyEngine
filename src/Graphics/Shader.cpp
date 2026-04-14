#include "TinyEngine/Graphics/Shader.h"

#include <fstream>
#include <sstream>
#include <utility>

#if defined(TINYENGINE_ENABLE_OPENGL)
#include <glad/glad.h>
#endif

namespace TinyEngine::Graphics {
	namespace {
		std::string ReadTextFile(std::string_view path) {
			std::ifstream inFile{std::string(path)};
			if (!inFile.is_open()) {
				return {};
			}

			std::ostringstream oss;
			oss << inFile.rdbuf();
			return oss.str();
		}

#if defined(TINYENGINE_ENABLE_OPENGL)
		bool CompileStage(std::uint32_t stageType, std::string_view source, std::uint32_t& outShaderId, std::string& outError) {
			outShaderId = glCreateShader(stageType);
			if (outShaderId == 0) {
				outError = "glCreateShader failed.";
				return false;
			}

			const char* sourcePtr = source.data();
			const int sourceLength = static_cast<int>(source.size());
			glShaderSource(outShaderId, 1, &sourcePtr, &sourceLength);
			glCompileShader(outShaderId);

			int compileStatus = 0;
			glGetShaderiv(outShaderId, GL_COMPILE_STATUS, &compileStatus);
			if (compileStatus == GL_TRUE) {
				return true;
			}

			int logLength = 0;
			glGetShaderiv(outShaderId, GL_INFO_LOG_LENGTH, &logLength);
			std::string infoLog(static_cast<std::size_t>(logLength > 0 ? logLength : 1), '\0');
			glGetShaderInfoLog(outShaderId, logLength, nullptr, infoLog.data());

			outError = infoLog;
			glDeleteShader(outShaderId);
			outShaderId = 0;
			return false;
		}
#endif
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

#if defined(TINYENGINE_ENABLE_OPENGL)
		std::uint32_t vertexShaderId = 0;
		if (!CompileStage(GL_VERTEX_SHADER, vertexSource, vertexShaderId, m_lastError)) {
			return false;
		}

		std::uint32_t fragmentShaderId = 0;
		if (!CompileStage(GL_FRAGMENT_SHADER, fragmentSource, fragmentShaderId, m_lastError)) {
			glDeleteShader(vertexShaderId);
			return false;
		}

		const bool linked = LinkFromCompiledStages(vertexShaderId, fragmentShaderId);

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return linked;
#else
		m_lastError = "OpenGL backend is disabled. Enable TINYENGINE_ENABLE_OPENGL for real shader compilation.";
		return false;
#endif
	}

	bool Shader::LinkFromCompiledStages(std::uint32_t vertexShaderId, std::uint32_t fragmentShaderId) {
		Reset();
		m_lastError.clear();

#if defined(TINYENGINE_ENABLE_OPENGL)
		m_programId = glCreateProgram();
		if (m_programId == 0) {
			m_lastError = "glCreateProgram failed.";
			return false;
		}

		glAttachShader(m_programId, vertexShaderId);
		glAttachShader(m_programId, fragmentShaderId);
		glLinkProgram(m_programId);

		int linkStatus = 0;
		glGetProgramiv(m_programId, GL_LINK_STATUS, &linkStatus);

		glDetachShader(m_programId, vertexShaderId);
		glDetachShader(m_programId, fragmentShaderId);

		if (linkStatus == GL_TRUE) {
			return true;
		}

		int logLength = 0;
		glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &logLength);
		std::string infoLog(static_cast<std::size_t>(logLength > 0 ? logLength : 1), '\0');
		glGetProgramInfoLog(m_programId, logLength, nullptr, infoLog.data());

		m_lastError = infoLog;
		glDeleteProgram(m_programId);
		m_programId = 0;
		return false;
#else
		(void)vertexShaderId;
		(void)fragmentShaderId;
		m_lastError = "OpenGL backend is disabled.";
		return false;
#endif
	}

	void Shader::Bind() const {
#if defined(TINYENGINE_ENABLE_OPENGL)
		if (m_programId != 0) {
			glUseProgram(m_programId);
		}
#endif
	}

	void Shader::Unbind() const {
#if defined(TINYENGINE_ENABLE_OPENGL)
		glUseProgram(0);
#endif
	}

	void Shader::Reset() {
#if defined(TINYENGINE_ENABLE_OPENGL)
		if (m_programId != 0) {
			glDeleteProgram(m_programId);
		}
#endif
		m_programId = 0;
	}

	bool Shader::IsValid() const {
		return m_programId != 0;
	}

	std::uint32_t Shader::GetProgramId() const {
		return m_programId;
	}

	const std::string& Shader::GetLastError() const {
		return m_lastError;
	}
} // namespace TinyEngine::Graphics

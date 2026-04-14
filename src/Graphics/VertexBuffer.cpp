#include "TinyEngine/Graphics/VertexBuffer.h"

#include <utility>

#include <glad/glad.h>

namespace TinyEngine::Graphics {
	VertexBuffer::~VertexBuffer() {
		Reset();
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept : m_data(std::move(other.m_data)),
																m_strideBytes(other.m_strideBytes),
																m_bufferId(other.m_bufferId),
																m_uploaded(other.m_uploaded) {
		other.m_strideBytes = 0;
		other.m_bufferId = 0;
		other.m_uploaded = false;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
		if (this == &other) {
			return *this;
		}

		Reset();

		m_data = std::move(other.m_data);
		m_strideBytes = other.m_strideBytes;
		m_bufferId = other.m_bufferId;
		m_uploaded = other.m_uploaded;

		other.m_strideBytes = 0;
		other.m_bufferId = 0;
		other.m_uploaded = false;
		return *this;
	}

	bool VertexBuffer::SetData(const float* data, std::size_t floatCount, std::uint32_t strideBytes) {
		if (data == nullptr || floatCount == 0 || strideBytes == 0) {
			return false;
		}

		m_data.assign(data, data + floatCount);
		m_strideBytes = strideBytes;
		m_uploaded = false;
		return true;
	}

	bool VertexBuffer::UploadToGpu() {
		if (!IsValid()) {
			return false;
		}

		if (m_bufferId == 0) {
			glGenBuffers(1, &m_bufferId);
		}

		if (m_bufferId == 0) {
			return false;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
		glBufferData(
			GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(m_data.size() * sizeof(float)),
			m_data.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_uploaded = true;
		return true;
	}

	void VertexBuffer::Bind() const {
		if (m_bufferId != 0) {
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
		}
	}

	void VertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Reset() {
		if (m_bufferId != 0) {
			glDeleteBuffers(1, &m_bufferId);
		}

		m_data.clear();
		m_strideBytes = 0;
		m_bufferId = 0;
		m_uploaded = false;
	}

	bool VertexBuffer::IsValid() const {
		return !m_data.empty() && m_strideBytes > 0;
	}

	bool VertexBuffer::IsUploaded() const {
		return m_uploaded && m_bufferId != 0;
	}

	std::uint32_t VertexBuffer::GetBufferId() const {
		return m_bufferId;
	}

	const std::vector<float>& VertexBuffer::Data() const {
		return m_data;
	}

	std::size_t VertexBuffer::FloatCount() const {
		return m_data.size();
	}

	std::uint32_t VertexBuffer::StrideBytes() const {
		return m_strideBytes;
	}
} // namespace TinyEngine::Graphics

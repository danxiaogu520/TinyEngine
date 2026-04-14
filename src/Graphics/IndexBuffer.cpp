#include "TinyEngine/Graphics/IndexBuffer.h"

#include <utility>

#include <glad/glad.h>

namespace TinyEngine::Graphics {
	IndexBuffer::~IndexBuffer() {
		Reset();
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept : m_indices(std::move(other.m_indices)),
															 m_bufferId(other.m_bufferId),
															 m_uploaded(other.m_uploaded) {
		other.m_bufferId = 0;
		other.m_uploaded = false;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
		if (this == &other) {
			return *this;
		}

		Reset();

		m_indices = std::move(other.m_indices);
		m_bufferId = other.m_bufferId;
		m_uploaded = other.m_uploaded;

		other.m_bufferId = 0;
		other.m_uploaded = false;
		return *this;
	}

	bool IndexBuffer::SetData(const std::uint32_t* indices, std::size_t indexCount) {
		if (indices == nullptr || indexCount == 0) {
			return false;
		}

		m_indices.assign(indices, indices + indexCount);
		m_uploaded = false;
		return true;
	}

	bool IndexBuffer::UploadToGpu() {
		if (!IsValid()) {
			return false;
		}

		if (m_bufferId == 0) {
			glGenBuffers(1, &m_bufferId);
		}

		if (m_bufferId == 0) {
			return false;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(m_indices.size() * sizeof(std::uint32_t)),
			m_indices.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_uploaded = true;
		return true;
	}

	void IndexBuffer::Bind() const {
		if (m_bufferId != 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
		}
	}

	void IndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::Reset() {
		if (m_bufferId != 0) {
			glDeleteBuffers(1, &m_bufferId);
		}

		m_indices.clear();
		m_bufferId = 0;
		m_uploaded = false;
	}

	bool IndexBuffer::IsValid() const {
		return !m_indices.empty();
	}

	bool IndexBuffer::IsUploaded() const {
		return m_uploaded && m_bufferId != 0;
	}

	std::uint32_t IndexBuffer::GetBufferId() const {
		return m_bufferId;
	}

	const std::vector<std::uint32_t>& IndexBuffer::Data() const {
		return m_indices;
	}

	std::size_t IndexBuffer::IndexCount() const {
		return m_indices.size();
	}
} // namespace TinyEngine::Graphics

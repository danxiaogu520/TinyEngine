#include "TinyEngine/Graphics/VertexArray.h"

#include "TinyEngine/Graphics/IndexBuffer.h"
#include "TinyEngine/Graphics/VertexBuffer.h"

#include <utility>

#if defined(TINYENGINE_ENABLE_OPENGL)
#include <glad/glad.h>
#endif

namespace TinyEngine::Graphics {
	VertexArray::~VertexArray() {
		Reset();
	}

	VertexArray::VertexArray(VertexArray&& other) noexcept : m_vertexBuffer(std::move(other.m_vertexBuffer)),
															 m_indexBuffer(std::move(other.m_indexBuffer)),
															 m_arrayId(other.m_arrayId),
															 m_built(other.m_built) {
		other.m_arrayId = 0;
		other.m_built = false;
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
		if (this == &other) {
			return *this;
		}

		Reset();

		m_vertexBuffer = std::move(other.m_vertexBuffer);
		m_indexBuffer = std::move(other.m_indexBuffer);
		m_arrayId = other.m_arrayId;
		m_built = other.m_built;

		other.m_arrayId = 0;
		other.m_built = false;
		return *this;
	}

	void VertexArray::SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) {
		m_vertexBuffer = std::move(vertexBuffer);
		m_built = false;
	}

	void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) {
		m_indexBuffer = std::move(indexBuffer);
		m_built = false;
	}

	bool VertexArray::Build() {
		if (!IsValid()) {
			return false;
		}

#if defined(TINYENGINE_ENABLE_OPENGL)
		if (!m_vertexBuffer->UploadToGpu()) {
			return false;
		}

		if (!m_indexBuffer->UploadToGpu()) {
			return false;
		}

		if (m_arrayId == 0) {
			glGenVertexArrays(1, &m_arrayId);
		}

		if (m_arrayId == 0) {
			return false;
		}

		const std::uint32_t strideBytes = m_vertexBuffer->StrideBytes();
		if (strideBytes == 0 || strideBytes % sizeof(float) != 0) {
			return false;
		}

		const std::uint32_t components = strideBytes / static_cast<std::uint32_t>(sizeof(float));
		if (components == 0 || components > 4) {
			return false;
		}

		glBindVertexArray(m_arrayId);
		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			static_cast<GLint>(components),
			GL_FLOAT,
			GL_FALSE,
			static_cast<GLsizei>(strideBytes),
			nullptr);

		glBindVertexArray(0);
		m_vertexBuffer->Unbind();
		m_indexBuffer->Unbind();

		m_built = true;
		return true;
#else
		return false;
#endif
	}

	void VertexArray::Bind() const {
#if defined(TINYENGINE_ENABLE_OPENGL)
		if (m_arrayId != 0) {
			glBindVertexArray(m_arrayId);
		}
#endif
	}

	void VertexArray::Unbind() const {
#if defined(TINYENGINE_ENABLE_OPENGL)
		glBindVertexArray(0);
#endif
	}

	void VertexArray::Reset() {
#if defined(TINYENGINE_ENABLE_OPENGL)
		if (m_arrayId != 0) {
			glDeleteVertexArrays(1, &m_arrayId);
		}
#endif

		m_vertexBuffer.reset();
		m_indexBuffer.reset();
		m_arrayId = 0;
		m_built = false;
	}

	bool VertexArray::IsValid() const {
		return m_vertexBuffer != nullptr && m_indexBuffer != nullptr && m_vertexBuffer->IsValid() && m_indexBuffer->IsValid();
	}

	bool VertexArray::IsBuilt() const {
		return m_built && m_arrayId != 0;
	}

	std::uint32_t VertexArray::GetArrayId() const {
		return m_arrayId;
	}

	const std::shared_ptr<VertexBuffer>& VertexArray::GetVertexBuffer() const {
		return m_vertexBuffer;
	}

	const std::shared_ptr<IndexBuffer>& VertexArray::GetIndexBuffer() const {
		return m_indexBuffer;
	}

	std::size_t VertexArray::VertexFloatCount() const {
		if (m_vertexBuffer == nullptr) {
			return 0;
		}

		return m_vertexBuffer->FloatCount();
	}

	std::size_t VertexArray::IndexCount() const {
		if (m_indexBuffer == nullptr) {
			return 0;
		}

		return m_indexBuffer->IndexCount();
	}
} // namespace TinyEngine::Graphics

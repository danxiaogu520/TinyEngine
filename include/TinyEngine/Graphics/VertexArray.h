#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

namespace TinyEngine::Graphics {
	class IndexBuffer;
	class VertexBuffer;

	class VertexArray {
	public:
		VertexArray() = default;
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		VertexArray(VertexArray&& other) noexcept;
		VertexArray& operator=(VertexArray&& other) noexcept;

		void SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

		bool Build();

		void Bind() const;
		void Unbind() const;

		void Reset();

		[[nodiscard]] bool IsValid() const;
		[[nodiscard]] bool IsBuilt() const;
		[[nodiscard]] std::uint32_t GetArrayId() const;
		[[nodiscard]] const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const;
		[[nodiscard]] const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;

		[[nodiscard]] std::size_t VertexFloatCount() const;
		[[nodiscard]] std::size_t IndexCount() const;

	private:
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		std::uint32_t m_arrayId = 0;
		bool m_built = false;
	};
} // namespace TinyEngine::Graphics

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace TinyEngine::Graphics {
	class IndexBuffer {
	public:
		IndexBuffer() = default;
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;

		bool SetData(const std::uint32_t* indices, std::size_t indexCount);
		bool UploadToGpu();

		void Bind() const;
		void Unbind() const;

		void Reset();

		[[nodiscard]] bool IsValid() const;
		[[nodiscard]] bool IsUploaded() const;
		[[nodiscard]] std::uint32_t GetBufferId() const;
		[[nodiscard]] const std::vector<std::uint32_t>& Data() const;
		[[nodiscard]] std::size_t IndexCount() const;

	private:
		std::vector<std::uint32_t> m_indices;
		std::uint32_t m_bufferId = 0;
		bool m_uploaded = false;
	};
} // namespace TinyEngine::Graphics

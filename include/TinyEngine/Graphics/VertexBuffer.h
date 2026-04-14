#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace TinyEngine::Graphics {
	class VertexBuffer {
	public:
		VertexBuffer() = default;
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;

		bool SetData(const float* data, std::size_t floatCount, std::uint32_t strideBytes);
		bool UploadToGpu();

		void Bind() const;
		void Unbind() const;

		void Reset();

		[[nodiscard]] bool IsValid() const;
		[[nodiscard]] bool IsUploaded() const;
		[[nodiscard]] std::uint32_t GetBufferId() const;
		[[nodiscard]] const std::vector<float>& Data() const;
		[[nodiscard]] std::size_t FloatCount() const;
		[[nodiscard]] std::uint32_t StrideBytes() const;

	private:
		std::vector<float> m_data;
		std::uint32_t m_strideBytes = 0;
		std::uint32_t m_bufferId = 0;
		bool m_uploaded = false;
	};
} // namespace TinyEngine::Graphics

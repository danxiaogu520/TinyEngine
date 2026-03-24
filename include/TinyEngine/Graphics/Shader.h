#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace TinyEngine::Graphics {

class Shader {
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    bool LoadFromFiles(std::string_view vertexPath, std::string_view fragmentPath);
    bool LoadFromSource(std::string_view vertexSource, std::string_view fragmentSource);

    void Bind() const;
    void Unbind() const;

    void Reset();

    [[nodiscard]] bool IsValid() const;
    [[nodiscard]] std::uint32_t GetProgramId() const;
    [[nodiscard]] const std::string& GetLastError() const;

private:
    bool m_isLoaded = false;
    std::uint32_t m_programId = 0;
    std::string m_lastError;
};

} // namespace TinyEngine::Graphics

#pragma once

namespace TinyEngine::ECS {

struct TransformComponent {
    float x = 0.0f;
    float y = 0.0f;
};

struct VelocityComponent {
    float vx = 0.0f;
    float vy = 0.0f;
};

} // namespace TinyEngine::ECS

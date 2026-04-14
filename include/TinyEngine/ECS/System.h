#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "TinyEngine/ECS/Registry.h"

namespace TinyEngine::ECS {

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void Update(Registry& registry, double deltaTimeSeconds) = 0;
};

class SystemScheduler {
public:
    template <typename SystemType, typename... Args>
    SystemType& AddSystem(Args&&... args) {
        static_assert(std::is_base_of_v<ISystem, SystemType>, "SystemType must derive from ISystem.");

        auto system = std::make_unique<SystemType>(std::forward<Args>(args)...);
        SystemType* rawPointer = system.get();
        m_systems.push_back(std::move(system));
        return *rawPointer;
    }

    void Update(Registry& registry, const double deltaTimeSeconds) {
        for (auto& system : m_systems) {
            system->Update(registry, deltaTimeSeconds);
        }
    }

    [[nodiscard]] std::size_t Count() const {
        return m_systems.size();
    }

private:
    std::vector<std::unique_ptr<ISystem>> m_systems;
};

} // namespace TinyEngine::ECS

#pragma once

#include <type_traits>

namespace TinyEngine::ECS {
	struct IComponent {
		virtual ~IComponent() = default;
	};

	template <typename Derived> struct Component : IComponent {
		static_assert(std::is_class_v<Derived>, "Derived component type must be a class/struct.");
		using Self = Derived;
	};
} // namespace TinyEngine::ECS

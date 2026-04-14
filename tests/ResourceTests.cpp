#include <iostream>
#include <memory>

#include "TinyEngine/Resource/ResourceManager.h"

namespace {
	struct DummyResource {
		int value = 0;
	};

	int TestRegisterAndResolve() {
		TinyEngine::Resource::ResourceManager manager;
		auto resource = std::make_shared<DummyResource>();
		resource->value = 42;

		auto handle = manager.Register<DummyResource>("dummy/main", resource);
		if (!handle.IsValid()) {
			return 1;
		}

		resource.reset();
		auto resolved = manager.Resolve(handle);
		if (resolved == nullptr || resolved->value != 42) {
			return 1;
		}

		if (!manager.Contains<DummyResource>("dummy/main")) {
			return 1;
		}

		return 0;
	}

	int TestReplaceAndUnload() {
		TinyEngine::Resource::ResourceManager manager;

		auto first = std::make_shared<DummyResource>();
		first->value = 1;
		auto firstHandle = manager.Register<DummyResource>("dummy/reload", first);

		auto second = std::make_shared<DummyResource>();
		second->value = 2;
		auto secondHandle = manager.Register<DummyResource>("dummy/reload", second);

		if (firstHandle.Id() != secondHandle.Id()) {
			return 1;
		}

		auto latest = manager.Get<DummyResource>("dummy/reload");
		if (latest == nullptr || latest->value != 2) {
			return 1;
		}

		if (!manager.Unload<DummyResource>("dummy/reload")) {
			return 1;
		}

		if (manager.Contains<DummyResource>("dummy/reload")) {
			return 1;
		}

		return 0;
	}
} // namespace

int main() {
	if (TestRegisterAndResolve() != 0) {
		std::cerr << "Resource manager register/resolve test failed" << std::endl;
		return 1;
	}

	if (TestReplaceAndUnload() != 0) {
		std::cerr << "Resource manager replace/unload test failed" << std::endl;
		return 1;
	}

	std::cout << "All resource tests passed" << std::endl;
	return 0;
}

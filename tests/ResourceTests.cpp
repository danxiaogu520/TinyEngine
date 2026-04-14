#include <iostream>
#include <memory>
#include <filesystem>
#include <fstream>
#include <vector>

#include "TinyEngine/Resource/AudioCache.h"
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

	int TestLoadOrGetPreventsDuplicateCreation() {
		TinyEngine::Resource::ResourceManager manager;
		int loadCount = 0;

		auto firstHandle = manager.LoadOrGet<DummyResource>("dummy/cache", [&loadCount]() {
			++loadCount;
			auto resource = std::make_shared<DummyResource>();
			resource->value = 99;
			return resource;
		});

		auto secondHandle = manager.LoadOrGet<DummyResource>("dummy/cache", [&loadCount]() {
			++loadCount;
			auto resource = std::make_shared<DummyResource>();
			resource->value = 100;
			return resource;
		});

		if (!firstHandle.IsValid() || !secondHandle.IsValid()) {
			return 1;
		}

		if (loadCount != 1) {
			return 1;
		}

		if (firstHandle.Id() != secondHandle.Id()) {
			return 1;
		}

		auto cached = manager.Get<DummyResource>("dummy/cache");
		if (cached == nullptr || cached->value != 99) {
			return 1;
		}

		return 0;
	}

	void AppendU16LE(std::vector<std::uint8_t>& out, std::uint16_t value) {
		out.push_back(static_cast<std::uint8_t>(value & 0xFFu));
		out.push_back(static_cast<std::uint8_t>((value >> 8u) & 0xFFu));
	}

	void AppendU32LE(std::vector<std::uint8_t>& out, std::uint32_t value) {
		out.push_back(static_cast<std::uint8_t>(value & 0xFFu));
		out.push_back(static_cast<std::uint8_t>((value >> 8u) & 0xFFu));
		out.push_back(static_cast<std::uint8_t>((value >> 16u) & 0xFFu));
		out.push_back(static_cast<std::uint8_t>((value >> 24u) & 0xFFu));
	}

	std::filesystem::path CreateTestWavFile() {
		const std::filesystem::path output = std::filesystem::temp_directory_path() / "tinyengine_day17_test.wav";

		constexpr std::uint32_t sampleRate = 8000;
		constexpr std::uint16_t channels = 1;
		constexpr std::uint16_t bitsPerSample = 16;
		constexpr std::uint16_t blockAlign = channels * (bitsPerSample / 8);
		constexpr std::uint32_t byteRate = sampleRate * blockAlign;
		constexpr std::uint32_t sampleCount = 160;
		constexpr std::uint32_t dataSize = sampleCount * blockAlign;
		constexpr std::uint32_t riffChunkSize = 36 + dataSize;

		std::vector<std::uint8_t> wav;
		wav.reserve(44 + dataSize);

		wav.insert(wav.end(), {'R', 'I', 'F', 'F'});
		AppendU32LE(wav, riffChunkSize);
		wav.insert(wav.end(), {'W', 'A', 'V', 'E'});

		wav.insert(wav.end(), {'f', 'm', 't', ' '});
		AppendU32LE(wav, 16);
		AppendU16LE(wav, 1);
		AppendU16LE(wav, channels);
		AppendU32LE(wav, sampleRate);
		AppendU32LE(wav, byteRate);
		AppendU16LE(wav, blockAlign);
		AppendU16LE(wav, bitsPerSample);

		wav.insert(wav.end(), {'d', 'a', 't', 'a'});
		AppendU32LE(wav, dataSize);

		for (std::uint32_t i = 0; i < sampleCount; ++i) {
			const std::uint16_t pcm = static_cast<std::uint16_t>((i % 40) * 700);
			AppendU16LE(wav, pcm);
		}

		std::ofstream out(output, std::ios::binary | std::ios::trunc);
		if (!out.is_open()) {
			return {};
		}

		out.write(reinterpret_cast<const char*>(wav.data()), static_cast<std::streamsize>(wav.size()));
		if (!out.good()) {
			return {};
		}

		return output;
	}

	int TestAudioCacheLoadAndReuse() {
		const std::filesystem::path wavPath = CreateTestWavFile();
		if (wavPath.empty() || !std::filesystem::exists(wavPath)) {
			return 1;
		}

		TinyEngine::Resource::AudioCache cache;
		auto firstHandle = cache.LoadAudio(wavPath.string());
		auto secondHandle = cache.LoadAudio(wavPath.string());

		std::error_code removeError;
		std::filesystem::remove(wavPath, removeError);

		if (!firstHandle.IsValid() || !secondHandle.IsValid()) {
			return 1;
		}

		if (firstHandle.Id() != secondHandle.Id()) {
			return 1;
		}

		auto audio = cache.GetAudio(firstHandle.Key());
		if (audio == nullptr || !audio->IsValid()) {
			return 1;
		}

		if (audio->Channels() != 1 || audio->SampleRate() != 8000) {
			return 1;
		}

		if (!cache.UnloadAudio(firstHandle.Key())) {
			return 1;
		}

		if (cache.Contains(firstHandle.Key())) {
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

	if (TestLoadOrGetPreventsDuplicateCreation() != 0) {
		std::cerr << "Resource manager load-or-get cache test failed" << std::endl;
		return 1;
	}

	if (TestAudioCacheLoadAndReuse() != 0) {
		std::cerr << "Audio cache load/reuse test failed" << std::endl;
		return 1;
	}

	std::cout << "All resource tests passed" << std::endl;
	return 0;
}

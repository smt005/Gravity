// ◦ Xyz ◦
#pragma once

#include <filesystem>
#include <string>
#include <Log.h>

namespace Engine
{
	class FileManager final
	{
	public:
		FileManager();

		const std::filesystem::path& GetRoot();
		std::string ReadFileText(const std::filesystem::path& filePath);
		std::vector<std::byte> ReadFileBinary(const std::filesystem::path& filePath);

		template<typename T>
		std::vector<T> ReadFile(const std::filesystem::path& filePath)
		{
			const auto bytes = ReadFileBinary(filePath);

			if (bytes.size() % sizeof(T) != 0) {
				LOG("[FileManager::ReadFileBinary] File size ({} bytes) is not aligned to '{}' sizeof: {}. File: '{}'", bytes.size(), typeid(T).name(), sizeof(T), filePath);
				return {};
			}

			std::vector<T> result(bytes.size() / sizeof(T));
			std::memcpy(result.data(), bytes.data(), bytes.size());

			return result;
		}

	private:
		std::filesystem::path _rootPath;
	};
}

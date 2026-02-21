// ◦ Xyz ◦
#pragma once

#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <Log.h>

namespace Engine
{
	class FileManager final
	{
	public:
		FileManager() = delete;
		FileManager(std::string_view name);
		FileManager(std::string_view name, const std::filesystem::path& relativePath);

		const std::string& GetName() const;
		const std::filesystem::path& GetRoot() const;
		const std::filesystem::path& SetRoot(const std::filesystem::path& path);

		std::string ReadFileText(const std::filesystem::path& filePath) const;

		template<typename T>
		std::vector<T> ReadFile(const std::filesystem::path& filePath) const {
			const std::filesystem::path fullFilepath = _rootPath / filePath;

			if (!std::filesystem::exists(fullFilepath)) {
				LOG("[FileManager::ReadFileBinary] File not found: '{}': '{}'", filePath, fullFilepath);
				return {};
			}

			const std::uintmax_t byteSize = std::filesystem::file_size(fullFilepath);
			if (byteSize % sizeof(T) != 0) {
				LOG("[FileManager::ReadFileBinary] File size ({} bytes) is not aligned to '{}' sizeof: {}. File: '{}'", byteSize, typeid(T).name(), sizeof(T), filePath);
				return {};
			}

			const std::uintmax_t size = byteSize / sizeof(T);
			std::vector<T> buffer(size);

			std::ifstream file(fullFilepath, std::ios::binary);
			if (!file) {
				LOG("[FileManager::ReadFileBinary] Failed to open file: '{}': '{}'", filePath, fullFilepath);
				return {};
			}

			file.read(reinterpret_cast<char*>(buffer.data()), byteSize);

			return buffer;
		}
		
		bool WriteFile(const void* const data, size_t size, const std::filesystem::path& filePath) const;
		bool WriteFile(const std::string& text, const std::filesystem::path& filePath) const;

		template<typename T>
		bool WriteFile(const std::vector<T>& data, const std::filesystem::path& filePath) const
		{
			return WriteFile(data.data(), data.size() * sizeof(T), filePath);
		}

	public:
		static FileManager& Make(const std::string& name, const std::filesystem::path& relativePath = {});
		static FileManager& Get(const std::string& name);
		static bool IsExist(const std::string& name);

	private:
		const std::string _name;
		std::filesystem::path _rootPath;

	private:
		inline static std::unordered_map<std::string, FileManager> fileManagers;
	};
}

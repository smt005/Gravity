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

		template<std::ranges::range T>
		T ReadFile(const std::filesystem::path& filePath) const
		{
			T buffer;
			if (!ReadFile(buffer, filePath)) {
				LOG("[FileManager::ReadFile] Error read file: '{}': '{}'", filePath);
			}
			return buffer;
		}

		template<std::ranges::range T>
		bool ReadFile(T& buffer, const std::filesystem::path& filePath) const
		{
			const std::filesystem::path fullFilepath = _rootPath / filePath;

			if (!std::filesystem::exists(fullFilepath)) {
				LOG("[FileManager::ReadFile] File not found: '{}': '{}'", filePath, fullFilepath);
				return false;
			}

			const std::uintmax_t byteSize = std::filesystem::file_size(fullFilepath);
			using TypeElement = std::ranges::range_value_t<T>;

			if (byteSize % sizeof(TypeElement) != 0) {
				LOG("[FileManager::ReadFile] File size ({} bytes) is not aligned to '{}' sizeof: {}. File: '{}'", byteSize, typeid(TypeElement).name(), sizeof(TypeElement), filePath);
				return false;
			}

			const std::uintmax_t size = byteSize / sizeof(TypeElement);
			buffer.resize(size, {});
			std::ifstream file(fullFilepath, std::ios::binary);

			if (!file) {
				LOG("[FileManager::ReadFile] Failed to open file: '{}': '{}'", filePath, fullFilepath);
				return false;
			}

			file.read(reinterpret_cast<char*>(buffer.data()), byteSize);

			return true;
		}

		template<std::ranges::range T>
		bool WriteFile(const T& data, const std::filesystem::path& filePath)
		{
			const std::filesystem::path fullFilepath = _rootPath / filePath;
			std::filesystem::create_directories(fullFilepath.parent_path());
			std::ofstream out(fullFilepath);

			if (!out) {
				LOG("[FileManager::WriteTextFile] Failed write file: '{}': '{}'", filePath, fullFilepath);
				return false;
			}

			using TypeElement = std::ranges::range_value_t<T>;
;			const std::uintmax_t size = data.size() * sizeof(TypeElement);
			out.write(reinterpret_cast<const char*>(data.data()), size);

			return out.good();
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

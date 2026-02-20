// ◦ Xyz ◦

#include "FileManager.h"
#include <fstream>

using namespace Engine;

FileManager::FileManager()
{
    _rootPath = std::filesystem::current_path();
}

const std::filesystem::path& FileManager::GetRoot()
{
	return _rootPath;
}

std::string FileManager::ReadFileText(const std::filesystem::path& filePath)
{
    const std::filesystem::path fullFilepath = _rootPath / filePath;

    std::ifstream file(fullFilepath, std::ios::binary);
    if (!file) {
        LOG("[FileManager::readFileText] Failed to open file: '{}': '{}'", filePath, fullFilepath);
        return {};
    }

    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

std::vector<std::byte> FileManager::ReadFileBinary(const std::filesystem::path& filePath)
{
    const std::filesystem::path fullFilepath = _rootPath / filePath;

    if (!std::filesystem::exists(fullFilepath)) {
        LOG("[FileManager::ReadFileBinary] File not found: '{}': '{}'", filePath, fullFilepath);
        return {};
    }

    const std::uintmax_t size = std::filesystem::file_size(fullFilepath);
    std::vector<std::byte> buffer(size);

    std::ifstream file(fullFilepath, std::ios::binary);
    if (!file) {
        LOG("[FileManager::ReadFileBinary] Failed to open file: '{}': '{}'", filePath, fullFilepath);
        return {};
    }

    file.read(reinterpret_cast<char*>(buffer.data()), size);

    return buffer;
}

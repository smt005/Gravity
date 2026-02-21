// ◦ Xyz ◦

#include "FileManager.h"
#include <exception>

using namespace Engine;

FileManager::FileManager(std::string_view name)
    : _name(name)
    , _rootPath(std::filesystem::current_path())
{}

FileManager::FileManager(std::string_view name, const std::filesystem::path& relativePath)
    : _name(name)
    , _rootPath(std::filesystem::current_path() / relativePath)
{}

const std::string& FileManager::GetName() const
{
    return _name;
}

const std::filesystem::path& FileManager::GetRoot() const
{
	return _rootPath;
}

const std::filesystem::path& FileManager::SetRoot(const std::filesystem::path& path)
{
    _rootPath = path;
    return GetRoot();
}

std::string FileManager::ReadFileText(const std::filesystem::path& filePath) const
{
    const std::filesystem::path fullFilepath = _rootPath / filePath;

    std::ifstream file(fullFilepath, std::ios::binary);
    if (!file) {
        LOG("[FileManager::readFileText] Failed to open file: '{}': '{}'", filePath, fullFilepath);
        return {};
    }

    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

bool FileManager::WriteFile(const void* const data, size_t size, const std::filesystem::path& filePath) const
{
    const std::filesystem::path fullFilepath = _rootPath / filePath;
    std::filesystem::create_directories(fullFilepath.parent_path());

    std::ofstream out(fullFilepath);
    if (!out) {
        LOG("[FileManager::WriteTextFile] Failed write file: '{}': '{}'", filePath, fullFilepath);
        return false;
    }

    out.write(reinterpret_cast<const char*>(data), size);
    return out.good();
}

bool FileManager::WriteFile(const std::string& text, const std::filesystem::path& filePath) const
{
    return WriteFile(text.data(), text.size() * sizeof(char), filePath);
}

FileManager& FileManager::Make(const std::string& name, const std::filesystem::path& relativePath) {
    auto it = fileManagers.find(name);

    if (it == fileManagers.end()) {
        if (relativePath.empty()) {
            it = fileManagers.emplace(name, FileManager(name)).first;
        }
        else {
            it = fileManagers.emplace(name, FileManager(name, std::filesystem::current_path() / relativePath)).first;
        }

        // TODO: exception
    }
    else if (!relativePath.empty()) {
        it->second.SetRoot(std::filesystem::current_path() / relativePath);
    }

    return it->second;
}

FileManager& FileManager::Get(const std::string& name)
{
    const auto it = fileManagers.find(name);
    if (it == fileManagers.end()) {
        throw std::range_error(TO_STRING("[FileManager::Get] name: {} not founded.", name));
    }
    return it->second;
}

bool FileManager::IsExist(const std::string& name)
{
    return fileManagers.contains(name);
}

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

FileManager& FileManager::Make(const std::string& name, const std::filesystem::path& relativePath) {
    auto it = fileManagers.find(name);

    if (it == fileManagers.end()) {
        if (relativePath.empty()) {
            it = fileManagers.emplace(name, FileManager(name)).first;
        }
        else {
            it = fileManagers.emplace(name, FileManager(name, std::filesystem::current_path() / relativePath)).first;
        }
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
        throw std::range_error(TO_STRING("[FileManager::Get] name: {} not founded. Use FileManager::Make", name));
    }
    return it->second;
}

bool FileManager::IsExist(const std::string& name)
{
    return fileManagers.contains(name);
}

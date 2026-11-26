// ◦ Xyz ◦

#include "JsonHelper.h"
#include <Files/FileManager.h>

nlohmann::json Engine::LoadJson(const std::string& fileName, const std::string& fileSystemName)
{
	return LoadJson(std::filesystem::path(fileName));
}

nlohmann::json Engine::LoadJson(const std::filesystem::path& fileName, const std::string& fileSystemName)
{
	const std::string jsonText = FileManager::Get(fileSystemName).ReadTextFile(fileName);
	nlohmann::json jsonData = nlohmann::json::parse(jsonText);
	return jsonData;
}

bool Engine::SaveJson(const nlohmann::json& jsonData, const std::string& fileName, const std::string& fileSystemName)
{
#ifdef _DEBUG
	constexpr unsigned int indentStep = 2;
#else
	constexpr unsigned int indentStep = 0;
#endif

	return FileManager::Get(fileSystemName).WriteFile(jsonData.dump(2), fileName);
}

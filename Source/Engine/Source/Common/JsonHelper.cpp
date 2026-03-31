// ◦ Xyz ◦

#include "JsonHelper.h"
#include <Files/FileManager.h>

Json Engine::LoadJson(const std::string& fileName, const std::string& fileSystemName)
{
	const std::string jsonText = FileManager::Get(fileSystemName).ReadTextFile(fileName);
	Json jsonData = Json::parse(jsonText);
	return jsonData;
}

bool Engine::SaveJson(const Json& jsonData, const std::string& fileName, const std::string& fileSystemName)
{
#ifdef _DEBUG
	constexpr unsigned int indentStep = 2;
#elif
	constexpr unsigned int indentStep = 0;
#endif

	return FileManager::Get(fileSystemName).WriteFile(jsonData.dump(2), fileName);
}

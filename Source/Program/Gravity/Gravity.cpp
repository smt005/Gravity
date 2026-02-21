// ◦ Xyz ◦

#include "Gravity.h"
#include <nlohmann/json.hpp>
#include <FileManager/FileManager.h>
#include <Callback/VirtualKey.h>

#include "Temp/LogSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

using json = nlohmann::json;

bool Gravity::Init(std::string_view params)
{
	InitFileManagers();
	InitCallback();

	JsonTest();
	return true;
}

void Gravity::Update() {}
void Gravity::Draw() {}
void Gravity::OnResize() {}
void Gravity::OnClose() {}

void Gravity::InitFileManagers()
{
	Engine::FileManager::Make("write");
	Engine::FileManager::Make("base", "../../Source/Resources/Files");
}

void Gravity::InitCallback()
{
	using namespace Engine;

	Callback::Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		if (data.key == VirtualKey::ESCAPE) {
			exit(0);
		}
	});
}

void Gravity::JsonTest()
{
	using namespace Engine;

	const std::string jsonFileName = "JsonText.json";
	json jsonData;

	try
	{
		const std::string jsonText = FileManager::Get("base").ReadFile<std::string>(jsonFileName);
		jsonData = json::parse(jsonText);
	}
	catch (const std::exception& exc) {
		LOG("exc: {}", exc);
	}
	catch (...) {
		LOG("EXCEPTION");
	}

	const std::string text = jsonData.contains("text") ? jsonData["text"] : "null";

	/*int age = 0;
	if (jsonData.contains("intValue")) {
		age = jsonData["intValue"];
	}*/
	int age = jsonData.contains("intValue") ? jsonData["intValue"].get<int>() : 0;

	LOG("Json: text: {} intValue: {}", text, age);

	jsonData["intValue"] = ++age;
	jsonData["text"] = text + text;

	std::string jsonText = jsonData.dump(2);
	LOG("json: {}", jsonText);
	FileManager::Get("base").WriteFile(jsonText, jsonFileName);
}

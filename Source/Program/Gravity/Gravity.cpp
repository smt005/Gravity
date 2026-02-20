// ◦ Xyz ◦

#include "Gravity.h"
#include <Callback/VirtualKey.h>
#include <FileManager/FileManager.h>
#include "Temp/LogSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

bool Gravity::Init(std::string_view params)
{
	InitCallback();
	FileManagerTests();
	return true;
}

void Gravity::Update() {}
void Gravity::Draw() {}
void Gravity::OnResize() {}
void Gravity::OnClose() {}

void Gravity::InitCallback()
{
	using namespace Engine;

	Callback::Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		if (data.key == VirtualKey::ESCAPE) {
			exit(0);
		}
	});
}

void Gravity::FileManagerTests()
{
	using namespace Engine;
	
	static FileManager fm;

	LOG("FILE_MANAGER: root: {}", fm.GetRoot());

	const std::string filePath = "TestFile.txt";
	{
		std::string textFromFile = fm.ReadFileText(filePath);
		LOG("string '{}': '{}'", filePath, textFromFile);
	}

	{
		std::vector<std::byte> arrBytes = fm.ReadFileBinary(filePath);
		LOG("vector<std::byte> '{}': {}", filePath, arrBytes);
	}

	{
		auto arr = fm.ReadFile<int>(filePath);
		LOG("vector<int> '{}': {}", filePath, arr);
	}
	{
		auto arr = fm.ReadFile<double>(filePath);
		LOG("vector<double> '{}': {}", filePath, arr);
	}
}

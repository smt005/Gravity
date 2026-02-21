// ◦ Xyz ◦

#include "Gravity.h"
#include <filesystem>
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

template <typename T>
T Read(const std::string& filePath, const std::string& fsName) {
	using namespace Engine;

	T data;
	
	if (!FileManager::IsExist(fsName)) {
		LOG("sf: {} not exist.", fsName);
		return data;
	}

	auto& fs = FileManager::Get(fsName);
	data = fs.ReadFile<T>(filePath);
	//fs.ReadFile(data, filePath);

	//LOG("READ: '{}' '{}' '{}\n\t\t\tdata: '{}'", typeid(T).name(), fs.GetName(), fs.GetRoot(), data);
	LOG("READ: '{}' '{}\n\t\t\tdata: '{}'", fs.GetName(), fs.GetRoot(), data);
	return data;
}

template <typename T>
void Write(const T& data, const std::string& filePath, const std::string& fsName)
{
	using namespace Engine;

	if (!FileManager::IsExist(fsName)) {
		LOG("sf: {} not exist.", fsName);
		return;
	}

	auto& fs = FileManager::Get(fsName);

	if (!fs.WriteFile(data, filePath)) {
		LOG("WRITE fail: {}", data);
		return;
	}

	LOG("WRITE OK: {}", data);
}

void Gravity::FileManagerTests()
{
	using namespace Engine;

	FileManager::Make("base", "../../Source/Resources/Files");
	FileManager::Make("write");

	/*LOG("FILE_MANAGER: name: '{}' root: {}\n", FileManager::Get("base").GetName(), FileManager::Get("base").GetRoot());

	try {
		LOG("FILE_MANAGER: name: '{}' root: {}\n", FileManager::Get("write").GetName(), FileManager::Get("write").GetRoot());
	}
	catch (const std::range_error& err) {
		LOG(err);
		FileManager::Make("write");
		LOG("FILE_MANAGER: name: '{}' root: {}\n", FileManager::Get("write").GetName(), FileManager::Get("write").GetRoot());
	}
	catch (...) {
		LOG("EXCEPTION");
	}*/

	/*{
		auto data = Read<std::string>("Text.txt", "base");
		char ch = data.empty() ? '0' : data.back();
		++ch;
		data += ch;
		Write(data, "Text.txt", "base");
	}*/

	{
		auto data = Read<std::vector<int>>("Data.data", "base");
		//auto data = FileManager::Get("base").ReadFile<std::string>("Data.data");
		// auto data = FileManager::Get("base").ReadFile<std::vector<int>>("Data.data");
		//std::vector<double> data;
		//FileManager::Get("base").ReadFile(data, "Data.data");
		data.emplace_back(data.size());
		Write(data, "Data.data", "base");
	}
}

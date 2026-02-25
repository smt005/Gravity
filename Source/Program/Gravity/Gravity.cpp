// ◦ Xyz ◦

#include "Gravity.h"
#include <FileManager/FileManager.h>
#include <Callback/VirtualKey.h>
#include <Draw/Draw.h>

#include "../Temp/LogSpecification.h"
#include "../Temp/LogStlSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

float vertices[] = {
     0.0f,  0.5f, 0.0f,  // верх
    -0.5f, -0.5f, 0.0f,  // левый низ
     0.5f, -0.5f, 0.0f   // правый низ
};

bool Gravity::Init(std::string_view params)
{
	InitFileManagers();
	InitCallback();

    InitDraw();
	return true;
}

void Gravity::Update() {}
void Gravity::OnResize() {}
void Gravity::OnClose() {}

void Gravity::InitFileManagers()
{
	Engine::FileManager::Make("write");
	Engine::FileManager::Make("base", "../../Source/Resources/Files");
	LOG("FileManager: {}", Engine::FileManager::Get("base"));
}

void Gravity::Draw() {
    TestDraw();
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

void Gravity::InitDraw()
{
	using namespace Engine;

	Draw::Init();
	Draw::SetClearColor(0.3f, 0.6f, 0.9f);

}

void Gravity::TestDraw()
{
	using namespace Engine;

	Draw::ClearColor();
	Draw::Render();
}

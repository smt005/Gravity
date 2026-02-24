// ◦ Xyz ◦

#include "Gravity.h"
#include <FileManager/FileManager.h>
#include <Callback/VirtualKey.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "../Temp/LogStlSpecification.h"
#include "../Temp/LogSpecification.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

bool Gravity::Init(std::string_view params)
{
	InitFileManagers();
	InitCallback();

	Test();
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

void Gravity::Test()
{
	{
		glm::vec2 vec2 = { 1.f, 2.5f };
		LOG("vec2: {}", vec2);
		vec2 = glm::normalize(vec2);
		LOG("vec2: {}", vec2);
	}
	{
		glm::vec3 vec3 = { 1.f, 0.7f, 0.5f };
		LOG("vec3: {}", vec3);
		vec3 = glm::normalize(vec3);
		LOG("vec3: {}", vec3);
	}
	{
		glm::vec4 vec4 = { 1.f, 3.f, 0.7f, 1.6f };
		LOG("vec4: {}", vec4);
		vec4 = glm::normalize(vec4);
		LOG("vec4: {}", vec4);
	}
	{
		glm::mat4x4 mat4(1.f);
		LOG("mat4x4: {}", mat4);
		mat4[0][1] = 1.234f;
		mat4[1][2] = 1.534f;
		mat4[2][2] = 1.746f;
		LOG("mat4x4: {}", mat4);
	}
}

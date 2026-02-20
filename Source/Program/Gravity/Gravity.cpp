// ◦ Xyz ◦

#include "Gravity.h"
#include <Callback/VirtualKey.h>
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

Gravity::Gravity()
{
	InitCallback();
	LOG("Gravity::Gravity");
}

bool Gravity::Init(std::string_view params)
{
	LOG("Gravity::Init params: {}", params);
	return true;
}

void Gravity::Update()
{
}

void Gravity::Draw()
{
}

void Gravity::OnResize()
{
}

void Gravity::OnClose()
{
	LOG("Gravity::OnClose");
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

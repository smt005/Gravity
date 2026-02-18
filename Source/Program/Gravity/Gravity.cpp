// ◦ Xyz ◦

#include "Gravity.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

Gravity::Gravity()
{
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

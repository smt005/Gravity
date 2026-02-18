// ◦ Xyz ◦

#include "Gravity.h"
#include <Log.h>

Gravity::Gravity()
{
	LOG("Gravity::Gravity");
}

bool Gravity::Init(std::string_view params)
{
	LOG("Gravity::Init");
	return true;
}

void Gravity::Update()
{
	static size_t counter = 0;
	LOG("Gravity::Update: {}", ++counter);
}

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

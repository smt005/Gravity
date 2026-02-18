// ◦ Xyz ◦

#include "Gravity.h"
#include <iostream>

Gravity::Gravity()
{
	std::cout << "Gravity::Gravity" << std::endl;
}

bool Gravity::Init(std::string_view params)
{
	std::cout << "Gravity::Init" << std::endl;
	return true;
}

void Gravity::Update()
{
	static size_t counter = 0;
	std::cout << "Gravity::Update: " << ++counter << std::endl;
}

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

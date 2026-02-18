// ◦ Xyz ◦


#include "Core.h"
#include <iostream>

using namespace Engine;

int Core::Execution(std::string_view params)
{
	if (!instanceProgram) {
		std::cout << "[Core::Execution] Engine::instanceProgram nullptr" << std::endl;
		return 1;
	}
	
	if (!instanceProgram->Init(params)) {
		std::cout << "[Core::Execution] Program::Init fail." << std::endl;
		return 2;
	}
	
	size_t testCounter = 1000;
	
	while(testCounter-- > 0) {
		instanceProgram->Update();
	}
	
	return 0;
}

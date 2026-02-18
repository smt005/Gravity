// ◦ Xyz ◦


#include "Core.h"
#include <Log.h>

using namespace Engine;

int Core::Execution(std::string_view params)
{
	if (!instanceProgram) {
		LOG("[Core::Execution] Engine::instanceProgram nullptr. {}", 0);
		return 1;
	}
	
	if (!instanceProgram->Init(params)) {
		LOG("[Core::Execution] Program::Init fail. {}", 0);
		return 2;
	}
	
	size_t testCounter = 1000;
	
	while(testCounter-- > 0) {
		instanceProgram->Update();
	}
	
	return 0;
}

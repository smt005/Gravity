// ◦ Xyz ◦

#include "../Source/Core.h"

int main(int argc, char** argv)
{
	const std::string_view params(*argv);
	return Engine::Core::Execution(params);
}

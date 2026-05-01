// ◦ Xyz ◦

#include <windows.h>
#include "../Source/Core.h"

int main(int argc, char** argv)
{
	SetConsoleOutputCP(CP_UTF8);

	const std::string_view params(*argv);
	return Engine::Core::Execution(params);
}

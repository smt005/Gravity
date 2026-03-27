// ◦ Xyz ◦

#include "Common.h"
#include <Defines.h>

#ifdef WINDOWS_PLATFORM
#include <windows.h>
#endif

bool Engine::IsDebugging()
{
#ifdef WINDOWS_PLATFORM
	static const bool isDebuggerPresent = IsDebuggerPresent();
	return isDebuggerPresent;
#endif

	return false;
}

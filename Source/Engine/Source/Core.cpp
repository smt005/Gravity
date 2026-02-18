// ◦ Xyz ◦

#include "Core.h"
#include <windows.h>
#include <GLFW/glfw3.h>
#include <Log.h>

using namespace Engine;

#if _DEBUG
#define ApplicationInfo	" v.0.0 DEBUG [" __DATE__"  " __TIME__" ]"
#else
#define ApplicationInfo ""
#endif

GLFWwindow* glfwWindow = nullptr;

struct Settings {
	const std::string_view title = "Gravity";
	int screenWidth = 600;
	int screenHeight = 400;
} settings;

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
	
	return Main(params);
}

int Core::Main(std::string_view params)
{
	if (!glfwInit()) {
		LOG("[Core::Main] glfwInit fail.");
		return 1;
	}

	const std::string title = TO_STRING("{}_{}", settings.title, ApplicationInfo);
	glfwWindow = glfwCreateWindow(settings.screenWidth, settings.screenHeight, title.c_str(), nullptr, nullptr);
	if (!glfwWindow) {
		glfwTerminate();
		LOG("[Core::Main] glfwCreateWindow fail.");
		return 1;
	}

	glfwMakeContextCurrent(glfwWindow);
	glfwGetFramebufferSize(glfwWindow, &settings.screenWidth, &settings.screenHeight);

	instanceProgram->Init(params);
	instanceProgram->OnResize();

	MainLoop();
	
	instanceProgram->OnClose();

	glfwTerminate();
	glfwWindow = nullptr;

	return 0;
}

void Core::MainLoop()
{
	while (!glfwWindowShouldClose(glfwWindow)) {
		instanceProgram->Update();
		instanceProgram->Draw();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}
}

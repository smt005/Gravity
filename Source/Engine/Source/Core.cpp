// ◦ Xyz ◦

#include "Core.h"
#include <windows.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <Log.h>

using namespace Engine;

#if _DEBUG
#define ApplicationInfo	" v.0.0 DEBUG [" __DATE__"  " __TIME__" ]"
#else
#define ApplicationInfo ""
#endif

GLFWwindow* glfwWindow = nullptr;

void cursorPositionCallback(GLFWwindow* Window, double x, double y);
void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods);
void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void windowPosCallback(GLFWwindow* window, int left, int top);
void windowCloseCallback(GLFWwindow* window);
void windowScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

struct Settings {
	const std::string_view title = "Gravity";
	int screenLeft = 100;
	int screenTop = 100;
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

	glfwSetWindowPos(glfwWindow, settings.screenLeft, settings.screenTop);
	glfwSetCursorPosCallback(glfwWindow, cursorPositionCallback);
	glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);
	glfwSetKeyCallback(glfwWindow, keyCallback);
	glfwSetWindowSizeCallback(glfwWindow, windowSizeCallback);
	glfwSetWindowPosCallback(glfwWindow, windowPosCallback);
	glfwSetWindowCloseCallback(glfwWindow, windowCloseCallback);
	glfwSetScrollCallback(glfwWindow, windowScrollCallback);

	glfwMakeContextCurrent(glfwWindow);

	const int version = gladLoadGL(glfwGetProcAddress);
	LOG("GL GLAD_VERSION_MAJOR: {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

	glfwGetFramebufferSize(glfwWindow, &settings.screenWidth, &settings.screenHeight);
	glViewport(0, 0, settings.screenWidth, settings.screenHeight);

	instanceProgram->Init(params);
	instanceProgram->OnResize();

	MainLoop();

	glfwTerminate();
	glfwWindow = nullptr;

	return 0;
}

void Core::MainLoop()
{
	while (!glfwWindowShouldClose(glfwWindow)) {
		{
			static float color[] = { 0.3f, 0.6f, 0.9f };
			static float direct[] = { 0.001f, 0.001f, 0.001f };

			color[0] += direct[0];
			color[1] += direct[1];
			color[2] += direct[2];
			if (color[0] > 1.f) {
				color[0] = 1.f; direct[0] = -0.001f;
			}
			else if (color[0] < 0.f) {
				color[0] = 0.f; direct[0] = 0.001f;
			}
			if (color[1] > 1.f) {
				color[1] = 1.f; direct[1] = -0.001f;
			}
			else if (color[1] < 0.f) {
				color[1] = 0.f; direct[1] = 0.001f;
			}
			if (color[2] > 1.f) {
				color[2] = 1.f; direct[2] = -0.001f;
			}
			else if (color[2] < 0.f) {
				color[2] = 0.f; direct[2] = 0.001f;
			}

			glClearColor(color[0], color[1], color[2], 1.f); 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}


		instanceProgram->Update();
		instanceProgram->Draw();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}
}

void cursorPositionCallback(GLFWwindow* Window, double x, double y)
{
	LOG("cursorPositionCallback: [{}, {}]", static_cast<int>(x), static_cast<int>(y));
}

void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods)
{
	switch (Action)
	{
	case GLFW_PRESS: {
		LOG("mouseButtonCallback: GLFW_PRESS: {}", Button);
	} break;
	case GLFW_RELEASE: {
		LOG("mouseButtonCallback: GLFW_RELEASE: {}", Button);
	} break;
	}
}

void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
	switch (Action)
	{
	case GLFW_PRESS: {
		LOG("keyCallback: GLFW_PRESS: {}", Key);
	} break;

	case GLFW_RELEASE: {
		LOG("keyCallback: GLFW_RELEASE: {}", Key);
	} break;
	}
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
	instanceProgram->OnResize();
	LOG("windowSizeCallback [{}, {}]", width, height);
}

void windowPosCallback(GLFWwindow* window, int left, int top)
{
	instanceProgram->OnResize();
	LOG("windowPosCallback: [{}, {}]", left, top);
}

void windowCloseCallback(GLFWwindow* window)
{
	instanceProgram->OnClose();
	LOG("windowCloseCallback");
}

void windowScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	LOG("windowScrollCallback");
}

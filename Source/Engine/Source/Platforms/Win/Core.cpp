// ◦ Xyz ◦

#include "Core.h"
#include <windows.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <Screen.h>
#include <Callback/Callback.h>
#include <Files/FileManager.h>
#include <Files/Settings.h>
#include <GuiWindow/GuiWindows.h>
#include <nlohmann/json.hpp>
#include <Log.h>

using namespace Engine;

#if _DEBUG
#define ApplicationInfo	" DEBUG ["__DATE__" "__TIME__"]"
#else
#define ApplicationInfo ""
#endif

const std::string_view windowTitle = "Gravity";
GLFWwindow* glfwWindow = nullptr;

void CursorPosCallback(GLFWwindow* Window, double x, double y);
void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods);
void KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);
void WindowSizeCallback(GLFWwindow* window, int width, int height);
void WindowPosCallback(GLFWwindow* window, int left, int top);
void WindowCloseCallback(GLFWwindow* window);
void WindowScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void SaveSettings()
{
	auto& settings = Settings::Instance();
	auto& jsonData = settings.JsonData();

	jsonData["width"] = ScreenParams::Width();
	jsonData["height"] = ScreenParams::Height();
	jsonData["top"] = ScreenParams::Top();
	jsonData["left"] = ScreenParams::Left();

	settings.SaveSettings();
}

int Core::Execution(std::string_view params)
{
	if (!instanceProgram) {
		LOG("[Core::Execution] Engine::instanceProgram nullptr.");
		return 1;
	}
	
	return Main(params);
}

void Core::Close()
{
	glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
}

int Core::Main(std::string_view params)
{
	if (!glfwInit()) {
		LOG("[Core::Main] glfwInit fail.");
		return 2;
	}

	FileManager::Make("write");
	Settings& settings = Settings::Instance();
	settings.LoadSettings();
	ScreenParams::SetWidth(settings.width);
	ScreenParams::SetHeight(settings.height);
	ScreenParams::SetTop(settings.top);
	ScreenParams::SetLeft(settings.left);

	const std::string title = TO_STRING("{}{}", windowTitle, ApplicationInfo);
	glfwWindow = glfwCreateWindow(ScreenParams::Width(), ScreenParams::Height(), title.c_str(), nullptr, nullptr);
	if (!glfwWindow) {
		glfwTerminate();
		LOG("[Core::Main] glfwCreateWindow fail.");
		return 3;
	}

	glfwSetWindowPos(glfwWindow, ScreenParams::Left(), ScreenParams::Top());
	glfwSetCursorPosCallback(glfwWindow, CursorPosCallback);
	glfwSetMouseButtonCallback(glfwWindow, MouseButtonCallback);
	glfwSetKeyCallback(glfwWindow, KeyCallback);
	glfwSetWindowSizeCallback(glfwWindow, WindowSizeCallback);
	glfwSetWindowPosCallback(glfwWindow, WindowPosCallback);
	glfwSetWindowCloseCallback(glfwWindow, WindowCloseCallback);
	glfwSetScrollCallback(glfwWindow, WindowScrollCallback);
	glfwMakeContextCurrent(glfwWindow);

	gladLoadGL(glfwGetProcAddress);
	int width = ScreenParams::Width();
	int height = ScreenParams::Height();
	glfwGetFramebufferSize(glfwWindow, &width, &height);
	glViewport(0, 0, width, height);

	if (!GuiWindows::Init(glfwWindow)) {
		LOG("[Core::Main] GuiWindows::Init fail.");
		return 4;
	}

	if (!instanceProgram->Init(params)) {
		LOG("[Core::Main] Program::Init fail.");
		return 5;
	}

	instanceProgram->OnResize();

	Callback::InitCallback();

	MainLoop();

	instanceProgram->OnClose();
	GuiWindows::Cleanup();
	SaveSettings();

	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
	glfwWindow = nullptr;

	return 0;
}

void Core::MainLoop()
{
	while (!glfwWindowShouldClose(glfwWindow)) {
		Callback::Update();
		instanceProgram->Update(Engine::Callback::GetDeltaTime());
		GuiWindows::UpdateWindows(Callback::GetDeltaTime());

		instanceProgram->Draw();
		GuiWindows::RenderWindows();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}
}

void CursorPosCallback(GLFWwindow* Window, double x, double y)
{
	Engine::Callback::OnCursorPosCallback(x, y);
}

void MouseButtonCallback(GLFWwindow* Window, int button, int action, int)
{
	switch (action)
	{
	case GLFW_PRESS: {
		Engine::Callback::OnMouseButtonCallback(Callback::Type::PRESS_TAP, button);
	} break;
	case GLFW_RELEASE: {
		Engine::Callback::OnMouseButtonCallback(Callback::Type::RELEASE_TAP, button);
	} break;
	}
}

void KeyCallback(GLFWwindow* Window, int key, int, int action, int)
{
	switch (action)
	{
	case GLFW_PRESS: {
		Engine::Callback::OnKeyCallback(Callback::Type::PRESS_KEY, key);
	} break;

	case GLFW_RELEASE: {
		Engine::Callback::OnKeyCallback(Callback::Type::RELEASE_KEY, key);
	} break;
	}
}

void WindowScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Engine::Callback::OnScrollCallback(yoffset);
}

void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	ScreenParams::SetWidth(width);
	ScreenParams::SetHeight(height);

	glViewport(0, 0, width, height);
	instanceProgram->OnResize();
	GuiWindows::ResizeWindows();
}

void WindowPosCallback(GLFWwindow* window, int left, int top)
{
	ScreenParams::SetLeft(left);
	ScreenParams::SetTop(top);

	instanceProgram->OnResize();
	GuiWindows::ResizeWindows();
}

void WindowCloseCallback(GLFWwindow* window)
{
	/*instanceProgram->OnClose();

	SaveSettings();
	settings.width = ScreenParams::Width();
	settings.height = ScreenParams::Height();
	settings.top = ScreenParams::Top();
	settings.left = ScreenParams::Left();*/
}

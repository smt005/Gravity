// ◦ Xyz ◦

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Screen.h"

using namespace Engine;

/*Camera::Camera(const Type type = Type::PERSPECTIVE)
	: _fov(45.f)
	, _type(type)
	, _matProject(glm::mat4x4(1.f))
	, _matView(glm::mat4x4(1.f))
	, _matProjectView(glm::mat4x4(1.f))
	, _pos(0.f, 0.f, 1.f)
	, _direct(0.f, 0.f, -1.f)
	, _up(0.f, 1.f, 0.f)
	
{
	if (type == Type::PERSPECTIVE) {
		SetPerspective(10000.f, 1.f, _fov);
	}
	else {
		SetOrtho();
	}
}*/

Camera::Camera()
{
	SetPerspective(10000.f, 1.f, _fov);
}

Camera::Camera(const Type type)
	: _type(type)
{
	if (type == Type::PERSPECTIVE) {
		SetPerspective(10000.f, 1.f, _fov);
	}
	else {
		SetOrtho();
	}
}

void Camera::SetPerspective(float zFar, float zNear, float fov) {
	_zFar = zFar;
	_zNear = zNear;
	_fov = fov;
	
	_matProject = glm::perspective(fov, ScreenParams::Aspect(), _zNear, _zFar);
	_matView = glm::lookAt(_pos, _pos + _direct, _up);
	MakeProjectView();
}

void Camera::SetOrtho(float size, float zNear, float zFar) {
	if (ScreenParams::Aspect() > 1.f) {
		_matProject = glm::ortho(-size * ScreenParams::Aspect(), size * ScreenParams::Aspect(), -size, size, zNear, zFar);
	}
	else {
		_matProject = glm::ortho(-size, size, -size / ScreenParams::Aspect(), size / ScreenParams::Aspect(), zNear, zFar);
	}

	MakeProjectView();
}

void Camera::Resize()
{
	_matProject = glm::perspective(_fov, ScreenParams::Aspect(), _zNear, _zFar);
	_matView = glm::lookAt(_pos, _pos + _direct, _up);
	MakeProjectView();
}

void Camera::MakeProjectView() {
	_matProjectView = _matProject * _matView;
}

const glm::mat4x4& Camera::GetMatProjectView() const
{
	return _matProjectView;
}

Camera& Camera::GetCurrentCameraRef()
{
	if (!currentCamera) {
		currentCamera = std::make_shared<Camera>();
	}
	return *currentCamera;
}

Camera::Ptr& Camera::GetCamera(const std::string& name)
{
	return cameras.at(name);
}

Camera& Camera::GetCameraRef(const std::string& name)
{
	const auto& ptr = GetCamera(name);
	if (!ptr) {
		throw std::invalid_argument(TO_STRING("[Camera::GetCameraRef] fail by name: {}", name));
	}
	return *ptr;
}

void Camera::SetCurrentCamera(const std::string& name)
{
	currentCamera = GetCamera(name);
	currentCamera->Resize();
}

void Camera::SetCurrentCamera(const Ptr& camera)
{
	currentCamera = camera;
	currentCamera->Resize();
}

const float* Camera::CurrentMatProjectView()
{
	return glm::value_ptr(GetCurrentCameraRef()._matProjectView);
}

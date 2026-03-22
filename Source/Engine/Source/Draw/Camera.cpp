
#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <Files/Settings.h>
#include <Common/JsonHelper.h>
#include "Callback/Callback.h"

using namespace Engine;

void Camera::Init() {
	_matView = glm::lookAt(_pos, _pos + _direct, _up);
	Resize();
}

void Camera::SetPerspective(const float zFar, const float zNear, const float fov) {
	_matProject = glm::perspective(fov, Engine::ScreenParams::Aspect(), zNear, zFar);
	MakeProjectView();
}

void Camera::SetOrtho(const float size, float zNear, float zFar) {
	float aspect = Engine::ScreenParams::Aspect();

	if (aspect > 1.f) {
		_matProject = glm::ortho(-size * aspect, size * aspect, -size, size, zNear, zFar);
	}
	else {
		_matProject = glm::ortho(-size, size, -size / aspect, size / aspect, zNear, zFar);
	}

	MakeProjectView();
}

/*glm::vec3 Camera::corsorCoord() {
	//glm::vec2 mousePos = Engine::Callback::mousePos();
	glm::vec2 mousePos(Callback::GetMousePos()[0], Callback::GetMousePos()[1]);
	mousePos.x += Engine::ScreenParams::Left();
	mousePos.y += Engine::ScreenParams::Top();

	glm::vec3 wincoord = glm::vec3(mousePos.x - Engine::ScreenParams::Left(), (Engine::ScreenParams::Height() - mousePos.y) + Engine::ScreenParams::Top(), 1.0f);
	glm::vec4 viewport = glm::vec4(0, 0, Engine::ScreenParams::Width(), Engine::ScreenParams::Height());

	glm::vec3 coord = glm::unProject(wincoord, _matView, _matProject, viewport);

	glm::vec3 vecCursor(_pos.x - coord.x, _pos.y - coord.y, _pos.z - coord.z);
	vecCursor = normalize(vecCursor);

	PhysicPlane plane;
	plane.set(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));

	glm::vec3 objcoord = plane.crossVector(vecCursor, _pos);

	return objcoord;
}*/

// TODO: Объединить Load, Save
bool Camera::Load()
{
	if (auto* jsonData = Settings::Instance().JsonData("Cameras")) {
		return Load(*jsonData);
	}

	return false;
}

void Camera::Save()
{
	if (auto* jsonData = Settings::Instance().JsonData("Cameras", true)) {
		Save(*jsonData);
	}
}

bool Camera::Load(const nlohmann::json& data) {
	using Json = nlohmann::json;

	if (data.empty()) {
		return false;
	}

	try {
		Type type = Type::PERSPECTIVE;
		glm::vec3 pos = glm::vec3(2.f, 2.f, 1.f);
		glm::vec3 direct = glm::vec3(-0.524f, -0.514f, -0.679f);
		glm::vec3 up = glm::vec3(0.f, 0.f, 1.f);

		const std::string typeStr = Engine::GetJsonValue<std::string>("type", data);
		if (typeStr == "ortho" || typeStr == "Ortho" || typeStr == "ORTHO") { // TODO:
			type = Type::ORTHO;
		}
		else if (typeStr == "perspective" || typeStr == "Perspective" || typeStr == "PERSPECTIVE") {
			type = Type::PERSPECTIVE;
		}

		const auto posData = Engine::GetJsonValue<std::vector<float>>("pos", data);
		if (posData.size() >= 3) {
			pos.x = posData[0];
			pos.y = posData[1];
			pos.z = posData[2];

			// TODO:
			if (pos.length() == 0.f) {
				pos = glm::vec3( 2.f, 2.f, 1.f );
			}
		}

		const auto directData = Engine::GetJsonValue<std::vector<float>>("direct", data);
		if (directData.size() >= 3) {
			direct.x = directData[0];
			direct.y = directData[1];
			direct.z = directData[2];

			// TODO:
			if (direct.length() == 0.f) {
				direct = glm::vec3( -0.524f, -0.514f, -0.679f );
			}
		}

		const auto upData = Engine::GetJsonValue<std::vector<float>>("up", data);
		if (upData.size() >= 3) {
			up.x = upData[0];
			up.y = upData[1];
			up.z = upData[2];

			// TODO:
			if (up.length() == 0.f) {
				up.z = 1.f;
			}
		}

		_type = type;
		_pos = pos;
		_direct = direct;
		_up = up;
	}
	catch (...) {
		Init();
		return false;
	}

	Init();
	return true;
}

void Camera::Save(nlohmann::json& data) {
	using Json = nlohmann::json;

	data["type"] = _type == Type::PERSPECTIVE ? "perspective" : "ortho";

	if (_pos.length() == 0.f) {
		data.erase("pos");
	}
	else {
		data["pos"] = { _pos.x, _pos.y, _pos.z };
	}

	if (_direct.length() == 0.f) {
		data.erase("direct");
	}
	else {
		data["direct"] = { _direct.x, _direct.y, _direct.z };
	}

	if (_up.length() == 1.f) {
		data.erase("up");
	}
	else {
		data["up"] = { _up.x, _up.y, _up.z };
	}
}

// STATIC
Camera::Ptr Camera::_currentCameraPtr;

Camera& Camera::GetLink() {
	if (!_currentCameraPtr) {
		_currentCameraPtr = std::make_shared<Camera>();
	}
	return *_currentCameraPtr;
}

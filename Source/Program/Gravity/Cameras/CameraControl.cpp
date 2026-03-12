
#include "CameraControl.h"
#include <Core.h>
#include <Callback/VirtualKey.h>
#include <Log.h>

using namespace Engine;

CameraControl::~CameraControl() {
	if (_callbackPtr) {
		delete _callbackPtr;
		_callbackPtr = nullptr;
	}
}

// VIRTUAL
/*void CameraControl::Load(const Json::Value& data) {
	Camera::Load(data);

	const Json::Value& speedData = data["speed"];
	if (speedData.isNumeric()) {
		_speed = speedData.asFloat();
	}

	const Json::Value& angleSpeedData = data["angleSpeed"];
	if (angleSpeedData.isNumeric()) {
		_angleSpeed = angleSpeedData.asFloat();
	}
}

void CameraControl::Save(Json::Value& data) {
	Camera::Save(data);

	data["class"] = "CameraControl";
	data["speed"] = _speed;
	data["angleSpeed"] = _angleSpeed;
}*/

//...
void CameraControl::Enable(const bool state) {
	if (state) {
		if (!_callbackPtr) {
			MakeCallback();
		}
	} else {
		delete _callbackPtr;
		_callbackPtr = nullptr;
	}
}

void CameraControl::MakeCallback() {
	Callback::Add(Callback::Type::PINCH_KEY, [this](const Engine::Callback::EventData& data) {
			float kForce = 1.0;

			if (Engine::Callback::KeyPressed(Engine::VirtualKey::CONTROL)) {
				kForce = 0.2f;
				return;
			} else if (Engine::Callback::KeyPressed(Engine::VirtualKey::SHIFT)) {
				kForce = 5.f;
			}

			if (Engine::Callback::KeyPressed('W')) {
				Move(MoveDirect::FORVARD, kForce);
			}
			if (Engine::Callback::KeyPressed('S')) {
				Move(MoveDirect::BACK, kForce);
			}
			if (Engine::Callback::KeyPressed('D')) {
				Move(MoveDirect::RIGHT, kForce);
			}
			if (Engine::Callback::KeyPressed('A')) {
				Move(MoveDirect::LEFT, kForce);
			}
			if (Engine::Callback::KeyPressed('R')) {
				Move(MoveDirect::TOP, kForce);
			}
			if (Engine::Callback::KeyPressed('F')) {
				Move(MoveDirect::DOWN, kForce);
			}
		}
	);

	Add(Callback::Type::PINCH_TAP, [this](const Engine::Callback::EventData& data) {
		if (Callback::MouseButtonPressed(VirtualTap::RIGHT)) {
				glm::vec2 delta(-Callback::GetDeltaMousePos()[0], -Callback::GetDeltaMousePos()[1]);
				Rotate(delta);
			}

			if (Engine::Callback::MouseButtonPressed(VirtualTap::MIDDLE)) {
				glm::vec2 delta(Callback::GetDeltaMousePos()[0], Callback::GetDeltaMousePos()[1]);
				Move(delta * static_cast<float>(Callback::GetDeltaTime()));
			}
		}
	);
}

void CameraControl::Move(const MoveDirect direct, const float kForce) {
	glm::vec3 pos = Pos();
	const glm::vec3& directVector = Direct();
	float speed = _speed * kForce;

	switch (direct)
	{
		case MoveDirect::FORVARD:
			pos += directVector * speed;
			break;

		case MoveDirect::BACK:
			pos -= directVector * speed;
			break;

		case MoveDirect::LEFT:
			pos.x -= (directVector.y * speed);
			pos.y += (directVector.x * speed);
			break;

		case MoveDirect::RIGHT:
			pos.x += (directVector.y * speed);
			pos.y -= (directVector.x * speed);
			break;

		case MoveDirect::TOP:
			pos.z += abs(speed);
			break;

		case MoveDirect::DOWN:
			pos.z -= abs(speed);
			break;

		case MoveDirect::FORVARD_HORIZONT:
			pos.x += (directVector.x * speed);
			pos.y += (directVector.y * speed);
			break;

		case MoveDirect::BACK_HORIZONT:
			pos.x -= (directVector.x * speed);
			pos.y -= (directVector.y * speed);
			break;
	}

	SetPos(pos);
}

template <typename T>
void CameraControl::Move(const T& directVector, const float kForce) {
	glm::vec3 pos = Pos();
	pos += directVector.x * _speed * kForce;
	SetPos(pos);
}

void CameraControl::Rotate(const glm::vec2& angles) {
	if (angles.length() == 0) {
		LOG("DELTA: {}, {}", angles.x, angles.y);
	}
	glm::vec3 directVector = Direct();

	float angleY = asinf(directVector.z);
	float valueForAcos = directVector.y / cos(angleY);
	if (-1.f < valueForAcos && valueForAcos > 1.f) {
		return;
	}

	float angleX = valueForAcos <= 1.f ? acosf(valueForAcos) : 0.f;

	if (directVector.x < 0.f)
		angleX = glm::pi<float>() + (glm::pi<float>() - angleX);

	angleX = angleX - static_cast<double>(angles.x) * _angleSpeed;
	angleY = angleY + static_cast<double>(angles.y) * _angleSpeed;

	// TODO:
	if (angleY > (glm::pi<float>() / 2.0 - 0.25)) angleY = (glm::pi<float>() / 2.0 - 0.25);
	if (angleY < -(glm::pi<float>() / 2.0 - 0.25)) angleY = -(glm::pi<float>() / 2.0 - 0.25);

	directVector.x = static_cast<float>(sin(angleX) * cos(angleY));
	directVector.y = static_cast<float>(cos(angleX) * cos(angleY));
	directVector.z = static_cast<float>(sin(angleY));
	directVector = glm::normalize(directVector);


	SetDirect(directVector);
}

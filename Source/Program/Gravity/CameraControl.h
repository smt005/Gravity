#pragma once

#include <Draw/Camera.h>
#include <Callback/Callback.h>

class CameraControl final : public Engine::Camera, public Engine::Callback {
public:
	using Ptr = std::shared_ptr<CameraControl>;

	enum class MoveDirect {
		NONE,
		FORVARD, BACK, LEFT, RIGHT, TOP, DOWN,
		FORVARD_HORIZONT, BACK_HORIZONT
	};

public:
	CameraControl() {}
	CameraControl(const Engine::Camera::Type type) { Camera::_type = type; }
	~CameraControl();

	//void Load(const Json::Value& data) override;
	//void Save(Json::Value& data) override;

	void Enable(const bool state);
	void MakeCallback();
	void Move(const MoveDirect direct, const float kForce = 1.f);
	template <typename T>
	void Move(const T& directVector, const float kForce = 1.f);
	void Rotate(const glm::vec2& angles);

	const float SetSpeed(const float speed) {
		_speed = speed;
		return _speed;
	}

private:
	float _speed = 1.f;
	float _angleSpeed = 0.005f;
	Engine::Callback* _callbackPtr = nullptr;
};

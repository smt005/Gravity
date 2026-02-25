// ◦ Xyz ◦

#include "GravityCameras.h"
#include <Draw/Camera.h>
#include <Callback/Callback.h>
#include <Callback/VirtualKey.h>
#include <glm/ext/matrix_transform.hpp>

namespace cameras
{
	using namespace Engine;

	class CameraControl final : public Camera, public Callback {
	public:
		CameraControl() = default;
		CameraControl(bool value)
		{
			SetEnadle(value);
		}

		void SetEnadle(bool value)
		{
			if (value && !_callbackId) {
				InitCallback();
			}
			else {
				Callback::Remove(_callbackId);
				_callbackId = 0;
			}
		}
	private:
		void InitCallback() {
			Callback::Add(Callback::Type::PINCH_KEY, [this, offsetVec = glm::vec3()](const Callback::EventData&) mutable {
				if (Callback::KeyPressed('A')) {
					offsetVec.x = 1.f;
				}
				if (Callback::KeyPressed('D')) {
					offsetVec.x = -1.f;
				}
				if (Callback::KeyPressed('W')) {
					offsetVec.y = -1.f;
				}
				if (Callback::KeyPressed('S')) {
					offsetVec.y = 1.f;
				}
				if (Callback::KeyPressed('R')) {
					offsetVec.z = 1.f;
				}
				if (Callback::KeyPressed('F')) {
					offsetVec.z = -1.f;
				}

				if (offsetVec.length() != 0.f) {
					float speed = Callback::KeyPressed(VirtualKey::SHIFT) ? _speed * 3.f : _speed;
					glm::vec3 pos = GetPos();
					pos += offsetVec * speed;
					SetPos(pos);
					offsetVec.x = 0.f;
					offsetVec.y = 0.f;
					offsetVec.z = 0.f;
				}
				});
		};

	private:
		Callback::FunId _callbackId = 0;
		float _speed = 0.01f;
		float _angleSpeed = 0.001f;;
	};

	void MakeCameras()
	{
		using namespace Engine;

		Camera::MakeCamera<Camera>("Main");
		Camera::SetCurrentCamera("Main");
		Camera::GetCurrentCameraRef().SetPos({ 0.f, 0.f, 5.f });

		Camera::MakeCamera<CameraControl>("Control", true);
		Camera::SetCurrentCamera("Control");
		Camera::GetCurrentCameraRef().SetPos({ 0.f, 0.f, 5.f });
	}
}

// ◦ Xyz ◦

#include "GravityCameras.h"
#include <Draw/Camera.h>
#include <Callback/Callback.h>
#include <Callback/VirtualKey.h>
#include <glm/ext/matrix_transform.hpp>


namespace cameras
{
	using namespace Engine;

	/*class CameraControl final : public Camera, public Callback {
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

			Callback::Add(Callback::Type::MOVE, [this, firstEnter = false, offsetVec = glm::vec2()](const Callback::EventData& data) mutable {
				///////if (!firstEnter) {
					firstEnter = true;
					offsetVec.x = data.cursorPos.y;
					offsetVec.y = data.cursorPos.x;
				}
				const glm::vec2 angles = { data.cursorPos.y - offsetVec.x, data.cursorPos.x - offsetVec.y };
				offsetVec.x = data.cursorPos.y;
				offsetVec.y = data.cursorPos.x;
				Rotate(angles);///////
				offsetVec.x = data.cursorPos.deltaX;
				offsetVec.y = data.cursorPos.deltaY;
				Rotate(offsetVec);
				});

		};

		void Rotate(const glm::vec2& angles) {
			glm::vec3 directVector = GetDirect();

			float angleY = asinf(directVector.z);
			float valueForAcos = directVector.y / cos(angleY);
			if (-1.f > valueForAcos > 1.f) {
				return;
			}

			float angleX = valueForAcos <= 1.f ? acosf(valueForAcos) : 0.f;

			if (directVector.x < 0.f)
				angleX = glm::pi<float>() + (glm::pi<float>() - angleX);

			angleX = angleX - static_cast<double>(angles.x) * _angleSpeed;
			angleY = angleY + static_cast<double>(angles.y) * _angleSpeed;

			if (angleY > (glm::pi<float>() / 2.0 - 0.25)) angleY = (glm::pi<float>() / 2.0 - 0.25);
			if (angleY < -(glm::pi<float>() / 2.0 - 0.25)) angleY = -(glm::pi<float>() / 2.0 - 0.25);

			directVector.x = static_cast<float>(sin(angleX) * cos(angleY));
			directVector.y = static_cast<float>(cos(angleX) * cos(angleY));
			directVector.z = static_cast<float>(sin(angleY));
			glm::normalize(directVector);


			SetDirect(directVector);
		}
	private:
		Callback::FunId _callbackId = 0;
		float _speed = 0.01f;
		float _angleSpeed = 0.00001f;
	};*/

	void MakeCameras()
	{
		using namespace Engine;

		/*Camera::MakeCamera<Camera>("Main");
		Camera::SetCurrentCamera("Main");
		Camera::GetCurrentCameraRef().SetPos({ 0.f, 0.f, 5.f });

		Camera::MakeCamera<CameraControl>("Control", true);
		Camera::SetCurrentCamera("Control");
		Camera::GetCurrentCameraRef().SetPos({ 0.f, 0.f, 5.f });*/

		camera = Camera::MakeAndSet<CameraControl>();
		camera->Init();
		camera->SetPerspective(10000000.f, 1.f, 45.f);
		camera->SetPos({ 2.f, 2.f, 1.f });
		camera->SetDirect({ -0.524f, -0.514f, -0.679f });
		camera->SetSpeed(1.0);
		camera->Enable(true);
	}
}

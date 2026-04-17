// ◦ Xyz ◦

#include "GravityCameras.h"
#include <Draw/Camera.h>
#include <Callback/Callback.h>
#include <Callback/VirtualKey.h>
#include <glm/ext/matrix_transform.hpp>

namespace cameras
{
	void MakeCameras()
	{
		camera = Engine::Camera::MakeAndSet<CameraControl>();

		if (!camera->Load()) {
			camera->SetPos({ 2.f, 2.f, 1.f });
			camera->SetDirect({ -0.524f, -0.514f, -0.679f });
			camera->Init();
		}

		camera->SetPerspective(10000000.f, 1.f, 45.f);
		camera->SetSpeed(1.0);
		camera->Enable(true);
	}

	void ResetCamera()
	{
		auto& camera = Engine::Camera::GetLink();
		camera.SetPos({ 2.f, 2.f, 1.f });
		camera.SetDirect({ -0.524f, -0.514f, -0.679f });
		camera.Init();
	}
}

// ◦ Xyz ◦
#pragma once

#include "CameraControl.h"

namespace cameras
{
	void MakeCameras();
	void ResetCamera();
	inline static CameraControl::Ptr camera;
}

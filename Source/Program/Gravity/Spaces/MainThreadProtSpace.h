// ◦ Xyz ◦
#pragma once

#include "DefaultSpace.h"

class MainThreadProtSpace : public DefaultSpace
{
public:
	void Update() override;
	void UpdateColapse();
	void UpdateForce();
	void UpdateSpeed(float deltaTime);
	void UpdatePos(float deltaTime);
};

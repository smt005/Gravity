// ◦ Xyz ◦
#pragma once

#include "DefaultSpace.h"

class MainThreadSpace : public DefaultSpace
{
public:
	void Update() override;
	void UpdateColapse();
	void UpdateForce();
	void UpdateSpeed(float deltaTime);
	void UpdatePos(float deltaTime);
};

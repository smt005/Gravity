// ◦ Xyz ◦
#pragma once

#include "Space.h"

class MainThreadSpace : public Space
{
public:
	MainThreadSpace();

	void Update() override;
	void UpdateForce();
	void UpdateSpeed();
	void UpdatePos();
};

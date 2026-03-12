// ◦ Xyz ◦
#pragma once

#include "Space.h"

class MainThreadSpace : public Space
{
public:
	void Update() override;
	void UpdateForce();
	void UpdateSpeed();
	void UpdatePos();
};

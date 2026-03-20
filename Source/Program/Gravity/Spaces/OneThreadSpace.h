// ◦ Xyz ◦
#pragma once

#include "Space.h"

class OneThreadSpace : public Space
{
public:
	void Update() override;
	void UpdateForce();
	void UpdateSpeed();
	void UpdatePos();
};

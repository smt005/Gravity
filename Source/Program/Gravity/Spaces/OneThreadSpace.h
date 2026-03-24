// ◦ Xyz ◦
#pragma once

#include "Space.h"

class OneThreadSpace : public Space
{
public:
	void Update() override;
	void CollectDebugData() const override;
	void UpdateColapse();
	void UpdateForce();
	void UpdateSpeed(float deltaTime);
	void UpdatePos(float deltaTime);
	void GetDebugData();
};

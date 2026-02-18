// ◦ Xyz ◦
#pragma once

#include "../../../Source/Engine/Source/Core.h"

class Gravity final : public Engine::Program
{
public:
	using Uptr = std::unique_ptr<Gravity>;

	Gravity();
	bool Init(std::string_view params) override;
	void Update() override;
};

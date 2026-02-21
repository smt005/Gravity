// ◦ Xyz ◦
#pragma once

#include <Core.h>
#include <Callback/Callback.h>

class Gravity final : public Engine::Program, Engine::Callback
{
public:
	using Uptr = std::unique_ptr<Gravity>;

	Gravity() = default;
	bool Init(std::string_view params) override;
	void Update() override;
	void Draw() override;
	void OnResize() override;
	void OnClose() override;

private:
	void InitFileManagers();
	void InitCallback();

private:
	void JsonTest();
};

// ◦ Xyz ◦
#pragma once

#include <Core.h>
#include <Callback/Callback.h>

class Gravity final : public Engine::Program, Engine::Callback
{
public:
	using Uptr = std::unique_ptr<Gravity>;

	Gravity();
	bool Init(std::string_view params) override;
	void Update() override;
	void Draw() override;
	void OnResize() override;
	void OnClose() override;
	void Test();

private:
	void InitCallback();

	void InitCallbackTest();
	void DelCallback();

private:
	Engine::Callback* _callback = nullptr;
};

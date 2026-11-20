// ◦ Xyz ◦
#pragma once

#include <Core.h>
#include <Callback/Callback.h>
#include <Object/Model.h>

class Strategy final : public Engine::Program, Engine::Callback
{
public:
	using Uptr = std::unique_ptr<Strategy>;

	Strategy() = default;
	bool InitFileManagers(std::string_view params) override;
	bool Init(std::string_view params) override;
	void Update(double deltaTime) override;
	void Draw() override;
	void OnResize() override;
	void OnClose() override;

private:
	void InitCallback();

private:
	//Engine::Model::Ptr _modelPtr;
};

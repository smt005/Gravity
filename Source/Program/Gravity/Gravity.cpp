// ◦ Xyz ◦

#include "Gravity.h"
#include <Log.h>

Engine::Program::Uptr instanceProgram = Engine::Program::MakeProgram<Gravity>();

Gravity::Gravity()
{
	InitCallback();
	LOG("Gravity::Gravity");
}

bool Gravity::Init(std::string_view params)
{
	LOG("Gravity::Init params: {}", params);
	return true;
}

void Gravity::Update()
{
}

void Gravity::Draw()
{
}

void Gravity::OnResize()
{
}

void Gravity::OnClose()
{
	LOG("Gravity::OnClose");
}

void Gravity::InitCallback()
{
	using namespace Engine;

	_callback.Add(Callback::Type::PRESS_KEY, [callback = &_callback](const Callback::EventData& data) {
		
		if (data.key == VirtualKey::ESCAPE) {
			LOG("_callback PRESS_KEY: ESCAPE {}", data.key);
			//exit(0);
		}
		else if (data.key == VirtualKey::ENTER) {
			LOG("_callback PRESS_KEY: ENTER == {}", data.key);

			{
				callback->Add(Callback::Type::PRESS_KEY, [](const Callback::EventData& data) {
						LOG("DEFF _callback PRESS_KEY: {}", data.key);
					});
			}
		}
		else if (data.key == 'Q') {
			LOG("_callback PRESS_KEY: Q == {}", data.key);
		}
		else if (data.key == 'q') {
			LOG("_callback PRESS_KEY: q == {}", data.key);
		}
		else if (data.key == VirtualKey::VK_1) {
			LOG("_callback PRESS_KEY: VK_1 == {}", data.key);
		}
		else if (data.key == '1') {
			LOG("_callback PRESS_KEY: 1 == {}", data.key);
		}
		else {
			LOG("_callback PRESS_KEY: {}", data.key);
		}
	});

	_callback.Add(Callback::Type::MOVE, [](const Callback::EventData& data) {
		LOG("_callback MOVE: [{}, {}]", data.cursorPos.x, data.cursorPos.y);
	});

	_callback.Add(Callback::Type::SCROLL, [](const Callback::EventData& data) {
		LOG("_callback SCROLL: {}", data.scrollOffset);
	});

	_callback.Add(Callback::Type::PRESS_TAP, [](const Callback::EventData& data) {
		if (data.button == VirtualTap::LEFT) {
			LOG("_callback PRESS_TAP: LEFT == {}", data.button);
		}
		else {
			LOG("_callback PRESS_TAP: {}", data.button);
		}
	});
}

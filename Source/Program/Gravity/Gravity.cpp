// ◦ Xyz ◦

#include "Gravity.h"
#include <Callback/VirtualKey.h>
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
	Callback::Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		if (data.key == VirtualKey::ESCAPE) {
			exit(0);
		}
	});

	InitCallbackTest();
}

void Gravity::InitCallbackTest()
{
	using namespace Engine;
	static std::vector<Callback::FunId> funIds;

	if (!_callback) {
		_callback = new Callback();
	}

	/*_callback->Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		if (_callback->KeyPressed('Q') && data.key == 'W') {
			LOG("CALLBACK COMBINE PRESS_KEY: {}", data.key);
		}


		if (data.key == VirtualKey::ENTER) {
			Callback::FunId funId = _callback->Add(Callback::Type::PRESS_KEY, [count = funIds.size()](const Callback::EventData& data) {
					LOG("										NEW CALLBACK[{}] PRESS_KEY: {}", count, data.key);
			});

			funIds.emplace_back(funId);
			LOG("										ADD CALLBACK PRESS_KEY: ENTER == {} funId: {}", data.key, funId);
		}

		if (data.key >= '0' && data.key <= '9') {
			if (!funIds.empty()) {
				LOG("										REMOVE CALLBACK PRESS_KEY: NUM == {} funId: {} funIds: {}", data.key, funIds.back(), funIds.size());
				_callback->Remove(funIds.back());
				funIds.pop_back();
			}
			else {
				LOG("										REMOVE CALLBACK PRESS_KEY: NUM == {} funIds: EMPTY", data.key, funIds.size());
			}
		}

		if (data.key == 'D') {
			LOG("										REMOVE ALL CALLBACK PRESS_KEY: NUM == {}", data.key);
			DelCallback();
		}

		LOG("										CALLBACK PRESS_KEY: {}", data.key);
	});*/

	_callback->Add(Callback::Type::PRESS_KEY, [this](const Callback::EventData& data) {
		LOG("CALLBACK PRESS_KEY: {} >>>>>>>>>>>>>>>>>>>>>>>", data.key);
		});

	_callback->Add(Callback::Type::PINCH_KEY, [this](const Callback::EventData& data) {
		LOG("CALLBACK PINCH_KEY: {}", data.key);
		});

	_callback->Add(Callback::Type::RELEASE_KEY, [this](const Callback::EventData& data) {
		LOG("CALLBACK RELEASE_KEY: {} <<<<<<<<<<<<<<<<<<<<<<<", data.key);
		});

	_callback->Add(Callback::Type::MOVE, [](const Callback::EventData& data) {
		LOG("_callback MOVE: [{}, {}]", data.cursorPos.x, data.cursorPos.y);
	});

	_callback->Add(Callback::Type::SCROLL, [](const Callback::EventData& data) {
		LOG("_callback SCROLL: {}", data.scrollOffset);
	});

	_callback->Add(Callback::Type::PRESS_TAP, [this](const Callback::EventData& data) {
		if (data.button == VirtualTap::LEFT) {
			LOG("PRESS_TAP: LEFT == {}", data.button);
		}
		else {
			LOG("PRESS_TAP: {}", data.button);
		}
		});

	_callback->Add(Callback::Type::PINCH_TAP, [this](const Callback::EventData& data) {
		if (data.button == VirtualTap::LEFT) {
			LOG("PINCH_TAP: LEFT == {}", data.button);
		}
		else {
			LOG("PREPINCH_TAPSS_TAP: {}", data.button);
		}
		});

	_callback->Add(Callback::Type::RELEASE_TAP, [this](const Callback::EventData& data) {
		if (data.button == VirtualTap::LEFT) {
			LOG("RELEASE_TAP: LEFT == {}", data.button);
		}
		else {
			LOG("RELEASE_TAP: {}", data.button);
		}
		});
}

void Gravity::DelCallback()
{
	LOG("_callback: {} // 0", _callback);
	delete _callback;
	LOG("_callback: {} // 1", _callback);
	_callback = nullptr;
	LOG("_callback: {} // 2", _callback);
}

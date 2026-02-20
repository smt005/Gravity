#include "Callback.h"

using namespace Engine;

Callback::~Callback()
{
	Clear();
}

bool Callback::KeyPressed(char ch)
{
	return callbackPinchKey.contains(ch);
}

bool Callback::MouseButtonPressed(int button)
{
	return callbackPinchMouseButton.contains(button);
}

Callback::FunId Callback::Add(Type type, Fun&& fun)
{
	Fun* newFun = &_callbackFuns[type].emplace_back(std::forward<Fun>(fun));
	auto& callbacks = callbacksByEvent[static_cast<size_t>(type)];
	const auto itCallback = std::find(callbacks.begin(), callbacks.end(), this);

	if (itCallback == callbacks.end()) {
		callbacks.emplace_back(this);
	}

	return reinterpret_cast<FunId>(newFun);
}

void Callback::Remove(FunId funId)
{
	for (int type = 0; type < static_cast<short int>(Type::COUNT); ++type) {
		Remove(static_cast<Type>(type), funId);
	}
}

void Callback::Remove(Type type, FunId funId)
{
	auto& callbacks = callbacksByEvent[static_cast<size_t>(type)];
	const auto itCallback = std::find(callbacks.begin(), callbacks.end(), this);

	if (itCallback != callbacks.end()) {
		auto& funs = (*itCallback)->_callbackFuns[type];
		const auto itFun = std::find_if(funs.begin(), funs.end(), [funId](const Fun& fun) {
			return reinterpret_cast<FunId>(&fun) == funId;
		});

		if (itFun != funs.end()) {
			*itFun = nullptr;
		}
	}
}

void Callback::Clear()
{
	for (auto& callbacks : callbacksByEvent) {
		const auto itCallback = std::find(callbacks.begin(), callbacks.end(), this);

		if (itCallback != callbacks.end()) {
			*itCallback = nullptr;
		}
	}
}

void Callback::OnCursorPosCallback(double x, double y)
{
	currentEventData.cursorPos.x = x;
	currentEventData.cursorPos.y = y;
	IterationCallback(Type::MOVE, currentEventData);
}

void Callback::OnMouseButtonCallback(Type type, int button)
{
	currentEventData.mouseButton = button;

	if (type == Type::PRESS_TAP) {
		callbackPinchMouseButton.emplace(currentEventData.mouseButton);
	}
	else if (type == Type::RELEASE_TAP) {
		callbackPinchMouseButton.erase(currentEventData.mouseButton);
	}

	IterationCallback(type, currentEventData);
}

void Callback::OnKeyCallback(Type type, int key)
{
	currentEventData.key = static_cast<char>(key);

	if (type == Type::PRESS_KEY) {
		callbackPinchKey.emplace(currentEventData.key);
	}
	else if (type == Type::RELEASE_KEY) {
		callbackPinchKey.erase(currentEventData.key);
	}

	IterationCallback(type, currentEventData);
}

void Callback::OnScrollCallback(double offset)
{
	currentEventData.scrollOffset = offset;
	IterationCallback(Type::SCROLL, currentEventData);
}

void Callback::IterationCallback(Type type, EventData eventData)
{
	auto& callbacks = callbacksByEvent[static_cast<size_t>(type)];
	const size_t countCallback = callbacks.size();

	for (size_t iCallback = 0; iCallback < countCallback; ++iCallback) {
		Callback*& callback = callbacks[iCallback];
		if (!callback) {
			continue;
		}

		auto& listFun = callback->_callbackFuns[type];
		const size_t countFun = listFun.size();
		size_t iFun = 0;

		for (auto it = listFun.begin(); it != listFun.end(); ++it) {
			if (iFun++ < countFun && *it) {
				(*it)(eventData);
			}

			if (!callback) {
				break;
			}
		}
		
		if (callback) {
			listFun.erase(std::remove(listFun.begin(), listFun.end(), nullptr), listFun.end());
			
			if (listFun.empty()) {
				callback = nullptr;
			}
		}
	}

	callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), nullptr), callbacks.end());
}

void Callback::Update()
{
	for (auto key : callbackPinchKey) {
		currentEventData.key = static_cast<char>(key);
		IterationCallback(Type::PINCH_KEY, currentEventData);
	}

	for (auto button : callbackPinchMouseButton) {
		currentEventData.mouseButton = button;
		IterationCallback(Type::PINCH_TAP, currentEventData);
	}
}

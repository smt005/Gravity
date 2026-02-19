#include "Callback.h"

using namespace Engine;

Callback::~Callback()
{
	Clear();
}

void Callback::Add(Type type, Fun&& fun)
{
	switch (type) {
	case Type::PRESS_KEY: { AddTo(std::forward<Fun>(fun), _keyCallback, keyCallbacks); } break;
	case Type::MOVE: { AddTo(std::forward<Fun>(fun), _cursorPosCallback, cursorPosCallbacks); } break;
	case Type::SCROLL: { AddTo(std::forward<Fun>(fun), _scrollCallback, scrollCallbacks); } break;
	case Type::PRESS_TAP: { AddTo(std::forward<Fun>(fun), _mouseButtonCallback, mouseButtonCallbacks); } break;
	default: {};
	};
}

void Callback::Clear()
{
	RemoveFrom(_cursorPosCallback, cursorPosCallbacks);
	RemoveFrom(_mouseButtonCallback, mouseButtonCallbacks);
	RemoveFrom(_keyCallback, keyCallbacks);
	RemoveFrom(_scrollCallback, scrollCallbacks);
}

void Callback::AddTo(Fun&& fun, Fun& callback, std::vector<Fun*>& globalCallbacks)
{
	callback = std::forward<Fun>(fun);
	globalCallbacks.emplace_back(&callback);
}

void Callback::RemoveFrom(Fun& fun, std::vector<Fun*>& globalCallbacks)
{
	const auto it = std::find(globalCallbacks.begin(), globalCallbacks.end(), &fun);
	if (it != globalCallbacks.end()) {
		globalCallbacks.erase(it);
	}

	fun = 0;
}

void Callback::OnCursorPosCallback(double x, double y)
{
	eventData.cursorPos.x = x;
	eventData.cursorPos.y = y;

	for (Fun* callback : cursorPosCallbacks) {
		(*callback)(eventData);
	}
}

void Callback::OnMouseButtonCallback(int button)
{
	eventData.button = button;
	
	for (Fun* callback : mouseButtonCallbacks) {
		(*callback)(eventData);
	}
}

void Callback::OnKeyCallback(int key)
{
	eventData.key = static_cast<char>(key);

	for (Fun* callback : keyCallbacks) {
		(*callback)(eventData);
	}
}

void Callback::OnScrollCallback(double offset)
{
	eventData.scrollOffset = offset;

	for (Fun* callback : scrollCallbacks) {
		(*callback)(eventData);
	}
}

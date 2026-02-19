#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "VirtualKey.h"

namespace Engine
{
	class Callback final
	{
	public:
		enum class Type
		{
			NONE,
			PRESS_TAP,
			PINCH_TAP,
			RELEASE_TAP,
			PRESS_KEY,
			PINCH_KEY,
			RELEASE_KEY,
			MOVE,
			SCROLL
		};

		struct CursorPos {
			double x;
			double y;
		};

		union EventData {
			CursorPos cursorPos;
			char key;
			int button;
			double scrollOffset;
		};

		using Fun = std::function<void(const EventData&)>;

		Callback() = default;
		~Callback();

		void Add(Type type, Fun&& fun);
		void Clear();

	private:
		void AddTo(Fun&& fun, Fun& callback, std::vector<Fun*>& globalCallbacks);
		void RemoveFrom(Fun& fun, std::vector<Fun*>& globalCallbacks);

	public:
		static void OnCursorPosCallback(double x, double y);
		static void OnMouseButtonCallback(int button);
		static void OnKeyCallback(int key);
		static void OnScrollCallback(double offset);

	private:
		Fun _cursorPosCallback = 0;
		Fun _mouseButtonCallback = 0;
		Fun _keyCallback = 0;
		Fun _scrollCallback = 0;

	private:
		inline static std::vector<Fun*> cursorPosCallbacks;
		inline static std::vector<Fun*> mouseButtonCallbacks;
		inline static std::vector<Fun*> keyCallbacks;
		inline static std::vector<Fun*> scrollCallbacks;
		inline static EventData eventData;
	};
}

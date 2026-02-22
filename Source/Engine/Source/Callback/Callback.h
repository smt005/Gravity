#pragma once

#include <memory>
#include <functional>
#include <array>
#include <list>
#include <unordered_set>
#include <unordered_map>

namespace Engine
{
	class Callback
	{
	public:
		enum class Type
		{
			PRESS_TAP,
			PINCH_TAP,
			RELEASE_TAP,
			PRESS_KEY,
			PINCH_KEY,
			RELEASE_KEY,
			MOVE,
			SCROLL,
			COUNT
		};

		struct CursorPos {
			float x;
			float y;
		};

		union EventData {
			CursorPos cursorPos;
			char key;
			int mouseButton;
			float scrollOffset;
		};

		using Fun = std::function<void(const EventData&)>;
		using FunId = std::uintptr_t;

		Callback() = default;
		virtual ~Callback();
		Callback(const Callback&) = delete;
		Callback(Callback&&) = delete;
		void operator = (const Callback&) = delete;
		void operator = (Callback&&) = delete;

		FunId Add(Type type, Fun&& fun);
		void Remove(FunId funId);
		void Remove(Type type, FunId funId);
		void Clear();

	public:
		static void OnCursorPosCallback(double x, double y);
		static void OnMouseButtonCallback(Type type, int button);
		static void OnKeyCallback(Type type, int key);
		static void OnScrollCallback(double offset);
		static void IterationCallback(Type type, EventData eventData);
		static void Update();
		static bool KeyPressed(char ch);
		static bool MouseButtonPressed(int button);

	private:
		std::unordered_map<Type, std::list<Fun>> _callbackFuns;

	private:
		inline static std::array<std::vector<Callback*>, static_cast<size_t>(Type::COUNT)> callbacksByEvent;
		inline static std::unordered_set<char> callbackPinchKey;
		inline static std::unordered_set<int> callbackPinchMouseButton;
		inline static EventData currentEventData;
	};
}

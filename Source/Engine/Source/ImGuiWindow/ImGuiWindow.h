// ◦ Xyz ◦
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <list>
#include <Log.h>

namespace Engine
{
	class ImGuiWindow {
	public:
		using Ptr = std::shared_ptr<ImGuiWindow>;
		using Wptr = std::weak_ptr<ImGuiWindow>;

		ImGuiWindow() = delete;
		ImGuiWindow(std::string_view name);
		virtual ~ImGuiWindow() = default;

		virtual void Update(double dTime) {};
		virtual void Render() {};
		virtual void Resize() {}
		virtual void OnOpen() {};
		virtual void OnClose() {};

		const std::string& GetName() const;
		bool IsVisible() const;
		void SetVisible(bool state);
		bool IsFullScreen() const;
		void SetFullScreen(bool state);
		void Close();

	private:
		bool _visible;
		bool _fullScreen;
		std::string _name;

	public:
		template <typename T>
		static std::shared_ptr<T> MakeWindow(std::string_view name) {
			std::shared_ptr<T> newWindow = std::make_shared<T>(name);
			const auto itPair = _windows.emplace(std::string(name), newWindow);
		
			if (!itPair.second) {
				throw(TO_STRING("[ImGuiWindow::Ptr Make] duplicate name: {} class: {}/", name, typeid(T).name()));
			}

			return newWindow;
		}

		static bool Init(void* window);
		static void Cleanup();
		static void RenderWindows();
		static void CloseWindow(const std::string& name);
		static void ResizeWindows();
		static void UpdateWindows(double dTime);
		static const Ptr& Get(const std::string name);
		static bool ExistWindow(const std::string& name);

	private:
		inline static std::unordered_map<std::string, Ptr> _windows;
		inline static std::list<Ptr> _windowToClose;
	};
}

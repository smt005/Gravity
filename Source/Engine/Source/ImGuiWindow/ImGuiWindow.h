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

	public:
		ImGuiWindow();
		ImGuiWindow(const std::string& name);
		virtual ~ImGuiWindow() = default;

		ImGuiWindow(const ImGuiWindow&) = delete;
		ImGuiWindow(ImGuiWindow&&) = delete;
		void operator = (const ImGuiWindow&) = delete;
		void operator = (ImGuiWindow&&) = delete;

		virtual void Render() = 0;
		virtual void Update(double dTime) {};
		virtual void Resize() {}
		virtual void OnOpen() {};
		virtual void OnClose() {};
		virtual void SetFullScreen(bool state);

		const std::string& GetName() const;
		const std::string& GetTitle() const;
		void SetTitle(std::string_view title);
		bool IsVisible() const;
		void SetVisible(bool state);
		bool IsFullScreen() const;
		void Close();

	private:
		bool _visible = true;
		bool _fullScreen = false;
		int _flags = 1 << 5; // ImGuiWindowFlags::ImGuiWindowFlags_NoCollapse
		std::string _name;
		std::string _title;

	public:
		template <typename T, typename ...Args>
		static std::shared_ptr<T> MakeWindow(Args&&... args) {
			std::shared_ptr<T> newWindow = std::make_shared<T>(std::forward<Args>(args)...);
			const auto itPair = _windows.emplace(newWindow.get()->GetName(), newWindow);

			if (!itPair.second) {
				throw(TO_STRING("[ImGuiWindow::Ptr Make] duplicate name: {} class: {}.", "name", typeid(T).name()));
			}

			newWindow->OnOpen();
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

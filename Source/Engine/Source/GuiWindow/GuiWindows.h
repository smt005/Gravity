// ◦ Xyz ◦
#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "GuiWindow.h"
#include <StringUtils.h>

namespace Engine
{
	class GuiWindows final {
	public:
		template <typename T, typename ...Args>
		static std::shared_ptr<T> MakeWindow(Args&&... args) {
			std::shared_ptr<T> newWindow = std::make_shared<T>(std::forward<Args>(args)...);
			const auto itPair = _windows.emplace(newWindow.get()->GetName(), newWindow);
			_windowsOpen.emplace(newWindow.get()->GetName());

			if (!itPair.second) {
				throw(TO_STRING("[GuiWindows::MakeWindow] duplicate name: {} class: {}.", "name", typeid(T).name()));
			}

			return newWindow;
		}

		template <typename Twindow>
		static void SwitchVisibleWindow()
		{
			const std::string windowName = Engine::GetClassName<Twindow>();
			if (!ExistWindow(windowName)) {
				MakeWindow<Twindow>(windowName)->GetName();
			}
			else {
				CloseWindow(windowName);
			}
		}

		static bool Init(void* window);
		static void Cleanup();
		static void RenderWindows();
		static void CloseWindow(std::string_view name);
		static void CloseWindow(const std::string& name);
		static void CloseWindows();
		static void ResizeWindows(std::string_view name = {});
		static void UpdateWindows(double dTime);
		static const GuiWindow::Ptr& Get(const std::string name);
		static bool ExistWindow(std::string_view name);
		static bool ExistWindow(const std::string& name);

	private:
		inline static bool _allWindowsResize = false;
		inline static std::unordered_set<std::string_view> _windowsResize;
		inline static std::unordered_set<std::string_view> _windowsOpen;
		inline static std::unordered_map<std::string, GuiWindow::Ptr> _windows;
	};
}

// ◦ Xyz ◦
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <list>
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

			if (!itPair.second) {
				throw(TO_STRING("[GuiWindows::MakeWindow] duplicate name: {} class: {}.", "name", typeid(T).name()));
			}

			return newWindow;
		}

		static bool Init(void* window);
		static void Cleanup();
		static void RenderWindows();
		static void CloseWindow(std::string_view name);
		static void CloseWindow(const std::string& name);
		static void ResizeWindows();
		static void UpdateWindows(double dTime);
		static const GuiWindow::Ptr& Get(const std::string name);
		static bool ExistWindow(std::string_view name);
		static bool ExistWindow(const std::string& name);

	private:
		inline static std::unordered_map<std::string, GuiWindow::Ptr> _windows;
		inline static std::list< GuiWindow::Ptr> _windowToClose;
	};
}

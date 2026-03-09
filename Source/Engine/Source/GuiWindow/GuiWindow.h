// ◦ Xyz ◦
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <list>
#include <StringUtils.h>

namespace Engine
{
	class GuiWindows;

	class GuiWindow {
		friend GuiWindows;

	public:
		using Ptr = std::shared_ptr<GuiWindow>;
		using Wptr = std::weak_ptr<GuiWindow>;

	public:
		GuiWindow();
		GuiWindow(const std::string& name);
		virtual ~GuiWindow() = default;

		GuiWindow(const GuiWindow&) = delete;
		GuiWindow(GuiWindow&&) = delete;
		void operator = (const GuiWindow&) = delete;
		void operator = (GuiWindow&&) = delete;

		virtual void Render() = 0;
		virtual void Update(double dTime) {};
		virtual void Resize() {}
		virtual void OnOpen() {};
		virtual void OnClose() {};
		virtual void SetFullScreen(bool state);

		const std::string& GetName() const;
		const std::string& GetTitle() const;
		const char* GetId() const;
		void SetTitle(std::string_view title);
		bool IsVisible() const;
		void SetVisible(bool state);
		int GetFlags() const;
		void SetFlags(int flag);
		int GetAlpha() const;
		void SetAlpha(int value);
		bool IsFullScreen() const;
		void Close();

	private:
		bool _opened = false;
		bool _visible = true;
		bool _fullScreen = false;
		float _alpha = 1.f;
		int _flags = 1 << 5; // ImGuiWindowFlags::ImGuiWindowFlags_NoCollapse
		std::string _name;
		std::string _title;
	};
}

// ◦ Xyz ◦
#pragma once

#include <memory>
#include <string>

namespace Engine
{
	class Program
	{
	public:
		using Uptr = std::unique_ptr<Program>;

		virtual ~Program() = default;
		virtual bool InitFileManagers(std::string_view params) = 0;
		virtual bool Init(std::string_view params) = 0;
		virtual void Update(double deltaTime) = 0;
		virtual void Draw() = 0;
		virtual void OnResize() = 0;
		virtual void OnClose() = 0;

		template<typename T>
		static Uptr MakeProgram()
		{
			return std::make_unique<T>();
		}
	};

	class Core final {
	public:
		static int Execution(std::string_view params);
		static void Close();

	private:
		static int Main(std::string_view params);
		static void MainLoop();
	};
}

extern Engine::Program::Uptr instanceProgram;

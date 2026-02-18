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
		virtual bool Init(std::string_view params) { return false; };
		virtual void Update() {};
		virtual void Draw() {};
		virtual void Resize() {};
		virtual void Close() {};

		template<typename T>
		static Uptr MakeProgram()
		{
			return std::make_unique<T>();
		}
	};

	class Core final {
	public:
		static int Execution(std::string_view params);
	};
}

extern Engine::Program::Uptr instanceProgram;

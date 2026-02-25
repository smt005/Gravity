// ◦ Xyz ◦
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <Log.h>

namespace Engine
{
	class Shader
	{
	public:
		using Ptr = std::shared_ptr<Shader>;

		Shader() = delete;
		Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
		virtual ~Shader();
		bool Load(const std::string& vertexFilePath, const std::string& fragmentFilePath);
		void UseProgram();

	private:
		unsigned int _program = 0; // GLuint

	public:
		template <typename T>
		static Ptr MakeShader(const std::string& name)
		{
			const std::string vertexFilePath = TO_STRING("Shaders/{}.vert", name);
			const std::string fragmentFilePath = TO_STRING("Shaders/{}.frag", name);

			try {
				const auto emplacePair = shaders.try_emplace(name, std::make_shared<T>(vertexFilePath, fragmentFilePath));
				return emplacePair.first->second;
			}
			catch (const std::exception& exc) {
				throw exc;
			}
		}

		static void UseProgram(const std::string& name);

	private:
		inline static std::unordered_map<std::string, Shader::Ptr> shaders;
	};
}

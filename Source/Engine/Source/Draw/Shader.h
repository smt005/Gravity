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
		Shader() = default;
		virtual ~Shader();
		virtual bool GetLocation() = 0;
		virtual bool UseProgram() = 0;

		void Load(const std::string& vertexFilePath, const std::string& fragmentFilePath);
		void LoadByName(const std::string& name);

	protected:
		unsigned int _program = 0; // GLuint
	};
}

// ◦ Xyz ◦
#pragma once

#include <Draw/Shader.h>
#include <Singletone.h>

namespace shaders
{
	class ColorShader final : public Engine::Shader, public mystd::Singletone<ColorShader>
	{
	public:
		void UseProgram() override;
		void GetLocation() override;
		void SetColor(const float* const color);

	private:
		unsigned int uMatProjectionView = 0;
		unsigned int uMatViewModel = 0;
		unsigned int uColor = 0;
	};

	void InitShaders();
}

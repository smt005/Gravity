// ◦ Xyz ◦
#pragma once

#include <Draw/Shader.h>
#include <Singletone.h>

namespace shaders
{
	class BaseShader : public Engine::Shader, public mystd::Singletone<BaseShader>
	{
	public:
		bool UseProgram() const override;
		bool GetLocation() override;
		void SetColor(const float* const color) const;

		template <std::ranges::range T>
		void SetColor(const T&& color) const {
			SetColor(color.data());
		}
		void SetModelPos(const float* const pos) const;
		void SetModelMatrix(const float* const mat) const;

	private:
		unsigned int uMatProjectionView = 0;
		unsigned int uMatViewModel = 0;
		unsigned int uColor = 0;
	};

	class LineShader : public Engine::Shader, public mystd::Singletone<LineShader>
	{
	public:
		bool UseProgram() const override;
		bool GetLocation() override;
		void SetColor(const float* const color) const;

		template <std::ranges::range T>
		void SetColor(const T&& color) const {
			SetColor(color.data());
		}
	private:
		unsigned int uMatProjectionView = 0;
		unsigned int uColor = 0;
	};

	void InitShaders();
}

// ◦ Xyz ◦
#pragma once

#include <Draw/Shader.h>
#include <Singletone.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace shaders
{
	//using Arr3 = float[3];

	class BaseShader : public Engine::Shader
	{
	public:
		bool UseProgram() const override;
		bool GetLocation() override;
		virtual void SetModelPos(const glm::vec3& pos) const;
		virtual void SetModelMatrix(const glm::mat4x4& mat) const;

		void SetColor(const float* const color) const;

		template <std::ranges::range T>
		void SetColor(const T&& color) const {
			SetColor(color.data());
		}


	protected:
		unsigned int uMatProjectionView = 0;
		unsigned int uMatViewModel = 0;
		unsigned int uColor = 0;
	};

	class ForwardShader : public BaseShader
	{
	public:
		bool UseProgram() const override;
		bool GetLocation() override;
		void SetModelPos(const glm::vec3& pos) const override;
		void SetModelMatrix(const glm::mat4x4& mat) const override;

	protected:
		unsigned int uCameraPos = 0;
		unsigned int uPos = 0;
	};

	class LineShader : public Engine::Shader
	{
	public:
		bool UseProgram() const override;
		bool GetLocation() override;
		void SetColor(const float* const color) const;

		template <std::ranges::range T>
		void SetColor(const T&& color) const {
			SetColor(color.data());
		}

	protected:
		unsigned int uMatProjectionView = 0;
		unsigned int uColor = 0;
	};

	class BaseShaderSingle : public BaseShader, public mystd::Singletone<BaseShaderSingle> {};
	class ForwardShaderSingle : public ForwardShader, public mystd::Singletone<ForwardShaderSingle> {};
	class LineShaderSingle : public LineShader, public mystd::Singletone<LineShaderSingle> {};

	void InitShaders();
}

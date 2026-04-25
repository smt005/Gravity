// ◦ Xyz ◦

#include "GravityShader.h"
#include <Draw/Shader.h>
#include <Draw/Camera.h>
#include <glad/gl.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Logs.h>

using namespace shaders;

// BaseShader

bool BaseShader::UseProgram() const
{
	if (!_program) {
		return false;
	}

	glUseProgram(_program);
	glUniformMatrix4fv(uMatProjectionView, 1, GL_FALSE, Engine::Camera::GetLink().ProjectViewFloat());

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	return true;
}

bool BaseShader::GetLocation()
{
	if (!_program) {
		return false;
	}

	uMatProjectionView = glGetUniformLocation(_program, "uMatProjectionView");
	uMatViewModel = glGetUniformLocation(_program, "uMatViewModel");
	uColor = glGetUniformLocation(_program, "uColor");

	return true;
}

void BaseShader::SetColor(const float* const color) const
{
	if (color) {
		glUniform4fv(uColor, 1, color);
	}
}

void BaseShader::SetModelPos(const glm::vec3& pos) const
{
	glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), pos);
	glUniformMatrix4fv(uMatViewModel, 1, GL_FALSE, glm::value_ptr(mat));
}

void BaseShader::SetModelMatrix(const glm::mat4x4&  mat) const
{
	glUniformMatrix4fv(uMatViewModel, 1, GL_FALSE, glm::value_ptr(mat));
}

// LineShader

bool LineShader::UseProgram() const
{
	if (!_program) {
		return false;
	}

	glBindVertexArray(0);

	glUseProgram(_program);
	glUniformMatrix4fv(uMatProjectionView, 1, GL_FALSE, Engine::Camera::GetLink().ProjectViewFloat());

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);

	return true;
}

bool LineShader::GetLocation()
{
	if (!_program) {
		return false;
	}

	uMatProjectionView = glGetUniformLocation(_program, "uMatProjectionView");
	uColor = glGetUniformLocation(_program, "uColor");

	return true;
}

void LineShader::SetColor(const float* const color) const
{
	if (color) {
		glUniform4fv(uColor, 1, color);
	}
}

// DisplayShader
bool DisplayShader::GetLocation()
{
	uTex = glGetUniformLocation(_program, "uTex");
	return true;
}

bool DisplayShader::UseProgram(unsigned int uTexture) const {
	if (Shader::UseProgram()) {
		glUniform1i(uTex, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, uTexture);

		return true;
	}

	return false;
}

void DisplayShader::EndProgram()
{
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
}


// AccumShader
bool AccumShader::GetLocation()
{
	uPrev = glGetUniformLocation(_program, "uPrev");
	uCurrent = glGetUniformLocation(_program, "uCurrent");
	uDecay = glGetUniformLocation(_program, "uDecay");
	return true;
}

bool AccumShader::UseProgram(float decay, unsigned int uPrevTexture, unsigned int uCurrentTexture) const {
	if (Shader::UseProgram()) {
		glUniform1i(uPrev, 0);
		glUniform1i(uCurrent, 1);
		glUniform1f(uDecay, decay);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, uPrevTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, uCurrentTexture);

		return true;
	}

	return false;
}

void AccumShader::EndProgram()
{
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
}

// InitShaders
void shaders::InitShaders()
{
	{
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.LoadByName("Texture");
		//shader.UseProgram();

		glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
		shader.SetColor(glm::value_ptr(color));

		glm::mat4x4 mat{ 1.f };
		shader.SetModelMatrix(mat);
	}

	{
		auto& shader = shaders::LineShaderSingle::Instance();
		shader.LoadByName("Line");
		shader.UseProgram();

		glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
		shader.SetColor(glm::value_ptr(color));
	}

	{
		AccumShaderSingle::Instance().LoadByName("Post/Accumulate");
		DisplayShaderSingle::Instance().LoadByName("Post/Display");
	}
}

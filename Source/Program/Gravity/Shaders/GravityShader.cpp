// ◦ Xyz ◦

#include "GravityShader.h"
#include <Draw/Shader.h>
#include <Draw/Camera.h>
#include <glad/gl.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Temp/LogSpecification.h>
#include <Log.h>

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


// ForwardShader

bool ForwardShader::UseProgram() const
{
	if (!BaseShader::UseProgram()) {
		return false;
	}

	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);

	glUniform3fv(uCameraPos, 1, glm::value_ptr(Engine::Camera::GetLink().Pos()));

	return true;
}

bool ForwardShader::GetLocation()
{
	if (!_program) {
		return false;
	}

	BaseShader::GetLocation();
	uCameraPos = glGetUniformLocation(_program, "uCameraPos");
	uPos = glGetUniformLocation(_program, "uPos");

	return true;
}

void ForwardShader::SetPos(const glm::vec3& pos)
{
	glUniform3fv(uPos, 1, glm::value_ptr(pos));
}

void ForwardShader::SetModelPos(const glm::vec3& pos) const
{
	glm::vec3 to = glm::normalize(Engine::Camera::GetLink().Pos() - pos);
	glm::vec3 from(0.f, 0.f, 1.f); 
	glm::vec3 axis = glm::normalize(glm::cross(from, to));

	// угол между векторами
	float dot = glm::dot(from, to);
	float angle = acos(dot);

	glm::mat4 mat(1.f);
	mat = glm::translate(mat, pos);

	if (glm::length(axis)) {
		mat = glm::rotate(mat, angle, axis);
	}

	

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

// InitShaders
void shaders::InitShaders()
{
	{
		auto& shader = shaders::BaseShaderSingle::Instance();
		shader.LoadByName("Texture");
		shader.UseProgram();

		glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
		shader.SetColor(glm::value_ptr(color));

		glm::mat4x4 mat{ 1.f };
		shader.SetModelMatrix(mat);
	}

	{
		auto& shader = shaders::ForwardShaderSingle::Instance();
		shader.LoadByName("Forward");
		shader.UseProgram();

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
}

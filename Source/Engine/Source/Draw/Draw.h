// ◦ Xyz ◦
#pragma once

#include <string>
#include "Shader.h"
#include "Object/Mesh.h"

namespace Engine
{
	enum class CullFaceType {
		NONE,
		FRONT,
		BACK,
		FRONT_AND_BACK,
		FRONT_LEFT,
		FRONT_RIGHT
	};

	class Draw final {
	public:
		static void SetClearColor(float r, float g, float b, float a = 1.f);
		static void ClearColor();
		static void ClearDepth();
		static void DepthTest(bool enable);
		static void CullFace(CullFaceType type);
		static void Viewport();
		static void SetPointSize(const float sizePoint);
		static void BindTexture(unsigned int textureId);
		static void Render(unsigned int vao, int count);
		static void RenderLines(const float* vertexes, unsigned int count);
		static void RenderPoints(const float* vertexes, unsigned int count);

		template <typename T>
		static void Render(const T& mesh) {
			Render(mesh.Vao(), mesh.Count());
		}

		// TODO:
		static void DrawToScreen();

	private:
		inline static float clearColor;
	};
}

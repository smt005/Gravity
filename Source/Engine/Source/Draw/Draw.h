// ◦ Xyz ◦
#pragma once

#include <string>
#include "Shader.h"
#include "Mesh.h"

namespace Engine
{
	class Draw final {
	public:
		static void SetClearColor(float r, float g, float b, float a = 1.f);
		static void ClearColor();
		static void ClearDepth();
		static void DepthTest(bool enable);
		static void Viewport();
		static void Render(unsigned int vao, int count);
		static void BindTexture(unsigned int textureId);

		template <typename T>
		static void Render(const T& mesh) {
			Render(mesh.Vao(), mesh.Count());
		}

	private:
		inline static float clearColor;
	};
}

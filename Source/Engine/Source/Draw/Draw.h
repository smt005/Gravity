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

		static void Init();
		static void Render(Mesh& mesh);

	private:
		inline static float clearColor;

	};
}

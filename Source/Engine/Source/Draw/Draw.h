// ◦ Xyz ◦
#pragma once

#include <string>
#include <functional>
#include <Draw/Shader.h>
#include <Object/Mesh.h>
#include <Object/Texture.h>

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

	struct Color {
		union {
			struct {
				float r, g, b, a;
			};
			float data[4] = { 0.f ,0.f ,0.f ,1.f };
		};
		operator float* () {
			return data;
		}
	};

	class Draw final {
	public:
		static void SetClearColor(float r, float g, float b, float a = 1.f);
		static void ClearColor();
		static void ClearColor(float r, float g, float b, float a = 1.f);
		static void ClearDepth();
		static void DepthTest(bool enable);
		static void CullFace(CullFaceType type);
		static void Viewport();
		static void SetPointSize(const float sizePoint);
		static void BindTexture(Texture& texture);
		static void BindTexture(unsigned int textureId);
		static void Render(unsigned int vao, int count);
		static void RenderTriangleFun(unsigned int vao, int count);
		static void RenderLines(const float* vertexes, unsigned int count);
		static void RenderPoints(const float* vertexes, unsigned int count);

		static void ActiveTexture(unsigned int texture);
		
		template <typename T>
		static void Render(const T& mesh) {
			Render(mesh.Vao(), mesh.Count());
		}

	private:
		inline static float clearColor;
	};
}

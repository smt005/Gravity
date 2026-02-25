// ◦ Xyz ◦
#pragma once
namespace Engine
{
	class Draw final {
	public:
		static void SetClearColor(float r, float g, float b, float a = 1.f);
		static void ClearColor();
		void ClearDepth();
		void DepthTest(bool enable);

		static void Init();
		static void Render();

	private:
		inline static float clearColor;

	};
}

// ◦ Xyz ◦
#pragma once

namespace Engine
{
	class ScreenParams {
	public:
		static int SetLeft(int value) {
			left = value;
			return left;
		}
		static int SetTop(int value) {
			top = value;
			return top;
		}
		static int SetWidth(int value) {
			width = value;
			SetAspect();
			return width;
		}
		static int SetHeight(int value) {
			height = value;
			SetAspect();
			return height;
		}
		
		static int Left() {
			return left;
		}
		static int Top() {
			return top;
		}
		static int Width() {
			return width;
		}
		static int Height() {
			return height;
		}

		static float Aspect() {
			return aspect;
		}

	private:
		static void SetAspect() {
			aspect = height != 0 ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;
		}

	private:
		inline static int left = 1400;
		inline static int top = 100;
		inline static int width = 1000;
		inline static int height = 600;
		inline static float aspect = 1.f;
	};
}

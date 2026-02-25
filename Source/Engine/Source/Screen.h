// ◦ Xyz ◦
#pragma once

namespace Engine
{
	struct ScreenParams {
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
			return width;
		}
		static int SetHeight(int value) {
			height = value;
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

		inline static int left = 1600;
		inline static int top = 100;
		inline static int width = 600;
		inline static int height = 400;
	};
}

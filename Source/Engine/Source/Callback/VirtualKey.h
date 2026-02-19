#pragma once

namespace Engine
{
	struct VirtualKey
	{
		inline static char VK_0 = 48;
		inline static char VK_1 = 49;
		inline static char VK_2 = 50;
		inline static char VK_3 = 51;
		inline static char VK_4 = 52;
		inline static char VK_5 = 53;
		inline static char VK_6 = 54;
		inline static char VK_7 = 55;
		inline static char VK_8 = 56;
		inline static char VK_9 = 57;
		inline static char TILDE = 96;

		inline static char ENTER = 1;
		inline static char SPACE = 32;
		inline static char ESCAPE = 256;
		inline static char RETURN = 259;

		inline static char RIGHT = 262;
		inline static char LEFT = 263;
		inline static char DOWN = 264;
		inline static char UP = 265;

		inline static char F1 = 290;
		inline static char F2 = 291;
		inline static char F3 = 292;
		inline static char F4 = 293;
		inline static char F5 = 294;
		inline static char F6 = 295;
		inline static char F7 = 296;
		inline static char F8 = 297;
		inline static char F9 = 298;
		inline static char F10 = 299;
		inline static char F11 = 300;
		inline static char F12 = 301;

		inline static char SHIFT = 340;
		inline static char CONTROL = 341;
	};

	struct VirtualTap
	{
		inline static int LEFT = 0;
		inline static int RIGHT = 1;
		inline static int MIDDLE = 2;
	};
}

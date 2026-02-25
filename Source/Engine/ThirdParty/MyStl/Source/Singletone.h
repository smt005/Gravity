#pragma once

namespace mystd
{
	template <typename T>
	class Singletone {
	public:
		static T& Instance()
		{
			if (!ptr) {
				ptr = new T();
			}

			return *ptr;
		}

		static void Clrean()
		{
			delete ptr;
			ptr = nullptr;
		}

	private:
		inline static T* ptr = nullptr;

	};
}

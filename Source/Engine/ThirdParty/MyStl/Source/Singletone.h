#pragma once

namespace mystd
{
	template <typename T>
	class Singletone {
	protected:
		Singletone() = default;
		Singletone(Singletone&) = default;
		Singletone(Singletone&&) = default;
		void operator = (Singletone&) {};
		void operator = (Singletone&&) {};
		~Singletone() = default;

	public:
		static T& Instance() {
			if (!ptr) {
				ptr = new T();
			}

			return *ptr;
		}

		inline static T& Inst() {
			return Instance();
		}

		static void Clrean() {
			if (ptr) {
				delete ptr;
				ptr = nullptr;
			}
		}

	private:
		inline static T* ptr = nullptr;
	};
}

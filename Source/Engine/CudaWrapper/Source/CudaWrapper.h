// ◦ Xyz ◦
#pragma once

#include <string>
#include <vector>
#include "../../../Program/Gravity/Spaces/BodyData.h"

namespace Cuda
{
	struct Vec3 {
		float x, y, z;
		Vec3() : x(0.f), y(0.f), z(0.f) {}
		Vec3(float value) : x(value), y(value), z(value) {}
		Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	};

	struct Body
	{
		float mass = 1.f;
		Vec3 pos;
		Vec3 velocity;
	};

	template <typename T>
	struct ValueWrapper
	{
		ValueWrapper(T _value = {});
		~ValueWrapper();

		operator T () {
			return value;
		}
		operator T* () {
			return valuePtr;
		}
		T GetValue() {
			return value;
		}
		void* Get() {
			return valuePtr;
		}

		T RetrieveValue();

	private:
		T value;
		T* valuePtr;
	};

	class CudaWrapper final {
	public:
		static void Calculate(std::vector<Body>& bodies);
	};
}

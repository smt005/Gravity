// ◦ Xyz ◦
#pragma once

#include <string>
#include <vector>
#include <iostream>
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

		Body() {}
		Body(float _mass, const Vec3& _pos, const Vec3& _velocity);
	};

	template <typename T>
	struct ValueWrapper
	{
		ValueWrapper();
		ValueWrapper(T _value);
		~ValueWrapper();

		operator T () const {
			return value;
		}
		operator T* () const {
			return valuePtr;
		}
		T GetValue() const {
			return value;
		}
		void* Get() const {
			return valuePtr;
		}

		T RetrieveData() const;

	private:
		mutable T value{};
		T* valuePtr = nullptr;
	};
	
	template <typename T>
	struct VectorWrapper
	{
		VectorWrapper(size_t _size);
		VectorWrapper(const std::vector<T>& data);
		VectorWrapper(std::vector<T>&& data) noexcept;
		~VectorWrapper();

		const std::vector<T>& operator = (const std::vector<T>& data);
		const std::vector<T>& operator = (std::vector<T>&& data) noexcept;

		/*operator const std::vector<T>() const {
			return data;
		}
		operator T* () const {
			return valuePtr;
		}*/
		const std::vector<T>& GetValue() const {
			return data;
		}
		void* Get() const {
			return valuePtr;
		}

		size_t Size() const {
			return data.size();
		};

		const std::vector<T>& RetrieveData() const;

	private:
		mutable std::vector<T> data;
		T* valuePtr = nullptr;
	};

	static std::ostream& operator<<(std::ostream& os, const Vec3& vec)
	{
		os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ']';
		return os;
	}

	static std::ostream& operator<<(std::ostream& os, const Body& body)
	{
		os << '{' << body.mass << '_' << body.pos << '_' << body.velocity << '}';
		return os;
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const ValueWrapper<T>& value)
	{
		os << value.RetrieveData();
		return os;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& os, const VectorWrapper<T>& wrapper)
	{
		const std::vector<T>& container = wrapper.RetrieveData();
		const size_t size = container.size();
		size_t number = 0;
		os << '[';
		for (auto&& item : container) {
			os << item;
			if (++number != size) {
				os << ',';
			}
		}
		os << ']';
		return os;
	}

	class CudaWrapper final {
	public:
		static void Calculate(VectorWrapper<Body>& bodies, ValueWrapper<float>& ans);
	};
}

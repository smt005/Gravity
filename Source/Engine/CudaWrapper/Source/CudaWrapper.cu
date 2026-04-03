// ◦ Xyz ◦

#include "CudaWrapper.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "../../ThirdParty/MyStl/Source/Log.h"

namespace Cuda {
	// Body
	Body:: Body(float _mass, const Vec3& _pos, const Vec3& _velocity)
		: mass(_mass)
		, pos(_pos)
		, velocity(_velocity)
	{}

	// ValueWrapper
	template <typename T>
	ValueWrapper<T>::ValueWrapper<T>()
	{
		cudaMalloc(&valuePtr, sizeof(T));
	}

	template <typename T>
	ValueWrapper<T>::ValueWrapper<T>(T _value)
		: value(_value)
	{
		cudaMalloc(&valuePtr, sizeof(T));
		cudaMemcpy(valuePtr, &value, sizeof(T), cudaMemcpyHostToDevice);
	}

	template <typename T>
	ValueWrapper<T>::~ValueWrapper<T>() {
		cudaFree(valuePtr);
	}

	template <typename T>
	T ValueWrapper<T>::RetrieveData() const
	{
		cudaMemcpy(&value, valuePtr, sizeof(T), cudaMemcpyDeviceToHost);
		return value;
	}

	// VectorWrapper
	template <typename T>
	VectorWrapper<T>::VectorWrapper<T>(size_t _size)
	{
		data.resize(_size);
		size = _size;
		cudaMalloc(&valuePtr, size * sizeof(T));
		cudaMemcpy(valuePtr, &data, size * sizeof(T), cudaMemcpyHostToDevice);
	}

	template <typename T>
	VectorWrapper<T>::VectorWrapper<T>(const std::vector<T>& _data)
		: data(_data)
		, size(data.size())
	{
		cudaMalloc(&valuePtr, size * sizeof(T));
		cudaMemcpy(valuePtr, &data, size * sizeof(T), cudaMemcpyHostToDevice);
	}

	template <typename T>
	VectorWrapper<T>::VectorWrapper<T>(std::vector<T>&& _data) noexcept
		: size(_data.size())
	{
		std::swap(data, _data);
		cudaMalloc(&valuePtr, size * sizeof(T));
		cudaMemcpy(valuePtr, &data, size * sizeof(T), cudaMemcpyHostToDevice);
	}

	template <typename T>
	VectorWrapper<T>::~VectorWrapper<T>() {
		cudaFree(valuePtr);
	}

	template <typename T>
	const std::vector<T>& VectorWrapper<T>::RetrieveData() const
	{
		cudaMemcpy(&data, valuePtr, size * sizeof(T), cudaMemcpyDeviceToHost);
		return data;
	}

	template struct ValueWrapper<int>;
	template struct ValueWrapper<float>;

	template struct VectorWrapper<int>;
	template struct VectorWrapper<float>;
	template struct VectorWrapper<Vec3>;
	template struct VectorWrapper<Body>;
}

using namespace Cuda;

__global__ void CudaFun(float a, float b, float* c)
{
	*c = *c * a * b;
}

void CudaWrapper::Calculate(std::vector<Body>& bodies, ValueWrapper<float>& ans)
{
	CudaFun << <1, 1 >> > (100.f, 5.f, ans);
}

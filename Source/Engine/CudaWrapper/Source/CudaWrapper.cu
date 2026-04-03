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
	ValueWrapper<T>::ValueWrapper()
	{
		cudaMalloc(&valuePtr, sizeof(T));
	}

	template <typename T>
	ValueWrapper<T>::ValueWrapper(T _value)
		: value(_value)
	{
		cudaMalloc(&valuePtr, sizeof(T));
		cudaMemcpy(valuePtr, &value, sizeof(T), cudaMemcpyHostToDevice);
	}

	template <typename T>
	ValueWrapper<T>::~ValueWrapper() {
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
	VectorWrapper<T>::VectorWrapper(size_t _size)
	{
		data.resize(_size);
		cudaMalloc(&valuePtr, data.size() * sizeof(T));
		cudaMemcpy(valuePtr, data.data(), data.size() * sizeof(T), cudaMemcpyHostToDevice);
	}

	template <typename T>
	VectorWrapper<T>::VectorWrapper(const std::vector<T>& _data)
		: data(_data)
	{
		cudaMalloc(&valuePtr, data.size() * sizeof(T));
		cudaMemcpy(valuePtr, data.data(), data.size() * sizeof(T), cudaMemcpyHostToDevice);
	}

	template <typename T>
	VectorWrapper<T>::VectorWrapper(std::vector<T>&& _data) noexcept
	{
		std::swap(data, _data);
		cudaMalloc(&valuePtr, data.size() * sizeof(T));
		cudaMemcpy(valuePtr, data.data(), data.size() * sizeof(T), cudaMemcpyHostToDevice);
	}

	template <typename T>
	const std::vector<T>& VectorWrapper<T>::operator = (const std::vector<T>& _data)
	{
		data = _data;
		if (valuePtr) {
			cudaFree(valuePtr);
			valuePtr = nullptr;
		}
		cudaMalloc(&valuePtr, data.size() * sizeof(T));
		cudaMemcpy(valuePtr, data.data(), data.size() * sizeof(T), cudaMemcpyHostToDevice);
		return data;
	}
	
	template <typename T>
	const std::vector<T>& VectorWrapper<T>::operator = (std::vector<T>&& _data) noexcept
	{
		std::swap(data, _data);
		if (valuePtr) {
			cudaFree(valuePtr);
			valuePtr = nullptr;
		}
		cudaMalloc(&valuePtr, data.size() * sizeof(T));
		cudaMemcpy(valuePtr, data.data(), data.size() * sizeof(T), cudaMemcpyHostToDevice);
		return data;
	}

	template <typename T>
	VectorWrapper<T>::~VectorWrapper() {
		cudaFree(valuePtr);
	}

	template <typename T>
	const std::vector<T>& VectorWrapper<T>::RetrieveData() const
	{
		cudaMemcpy(data.data(), valuePtr, data.size() * sizeof(T), cudaMemcpyDeviceToHost);
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

__global__ void CudaFun(Body* bodies, size_t size, float* value)
{
	float sumMass = 0;

	for (int index = 0; index < size; ++index) {
		Body* body = &bodies[index];
		body->pos.x = body->pos.x * *value;
	
		sumMass = sumMass + body->mass;
	}

	*value = sumMass;
}

void CudaWrapper::Calculate(VectorWrapper<Body>& bodies, ValueWrapper<float>& ans)
{
	CudaFun<<<1, 1>>>(static_cast<Body*>(bodies.Get()), bodies.Size(), static_cast<float*>(ans.Get()));
	cudaDeviceSynchronize();
}

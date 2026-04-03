// ◦ Xyz ◦

#include "CudaWrapper.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "../../ThirdParty/MyStl/Source/Log.h"

namespace Cuda {
	// ValueWrapper
	template <typename T>
	ValueWrapper<T>::ValueWrapper<T>(T _value)
		: value(_value)
	{
		cudaMalloc(&valuePtr, sizeof(T));
	}

	template <typename T>
	ValueWrapper<T>::~ValueWrapper<T>() {
		cudaFree(valuePtr);
	}

	template <typename T>
	T ValueWrapper<T>::RetrieveValue()
	{
		cudaMemcpy(&value, valuePtr, sizeof(T), cudaMemcpyDeviceToHost);
		return value;
	}
}

using namespace Cuda;

__global__ void CudaFun(float a, float b, float* c)
{
	*c = a * b;
}

void CudaWrapper::Calculate(std::vector<Body>& bodies)
{
	ValueWrapper<float> c;

	CudaFun << <1, 1 >> > (1000, 1.1f, c);

	LOG("ans: a * b = {}", c.RetrieveValue());
}

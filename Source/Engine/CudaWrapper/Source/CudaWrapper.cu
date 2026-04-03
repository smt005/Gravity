// ◦ Xyz ◦

#include "CudaWrapper.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "../../ThirdParty/MyStl/Source/Log.h"

using namespace Cuda;

__global__ void CudaFun(Body* bodies, size_t size, float* value)
{
	float sumMass = 0;

	for (int index = 0; index < size; ++index) {
		Body* body = &bodies[index];
		body->pos.x += *value;
		body->pos.y += *value;
		body->pos.z += *value;

		sumMass += body->mass;
	}

	*value = sumMass;
}

void CudaWrapper::Calculate(VectorWrapper<Body>& bodies, ValueWrapper<float>& ans)
{
	CudaFun<<<1, 1>>> (bodies, bodies.Size(), ans);
	cudaDeviceSynchronize();
}

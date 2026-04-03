// ◦ Xyz ◦
#pragma once

#include "CudaExample.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace Cuda;

__global__ void CudaExampleFun(Body* bodies, size_t size, float* value)
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

void ExampleCuda::Calculate(VectorWrapper<Body>& bodies, ValueWrapper<float>& value)
{
	CudaExampleFun <<<1,1>>> (bodies, bodies.Size(), value);
	cudaDeviceSynchronize();
}

/*
#include <CudaExample.h>
#include <Logs.h>

void CudaExampleFun()
{
	Cuda::ValueWrapper<float> valueW(10000);
	Cuda::VectorWrapper<Cuda::Body> dataW = std::vector<Cuda::Body>{ {1.f, {1.f, 2.f, 3.f}, {1.f, 2.f, 3.f}}, {2.f, {10.f, 20.f, 30.f}, {11.f, 22.f, 33.f}} };

	LOG("AFTER: valueW: '{}' dataW: {}", valueW, dataW);
	Cuda::CudaWrapper::Calculate(dataW, valueW);
	LOG("BEFORE: valueW: '{}' dataW: {}", valueW, dataW);
}
*/

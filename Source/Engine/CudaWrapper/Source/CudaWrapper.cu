// ◦ Xyz ◦

#include "CudaWrapper.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace Cuda {
	cudaDeviceProp deviceProp;

	int         deviceCount;
	bool processGPU;
	bool multithread;
	std::string nameGPU;
	int         warpSize;                    // Warp size in threads
	int         maxThreadsPerBlock;          // Maximum number of threads per block
	int         maxThreadsDim[3];                // Maximum size of each dimension of a block
	int         maxGridSize[3];                  // Maximum size of each dimension of a grid
	int         maxThreadsPerMultiProcessor; // Maximum resident threads per multiprocessor
	int         maxBlocksPerMultiProcessor;  // Maximum number of resident blocks per multiprocessor

	bool GetProperty() {
		if (deviceCount == -1) {
			cudaGetDeviceCount(&deviceCount);

			if (deviceCount == 0) {
				return false;
			}
		}
		cudaGetDeviceProperties(&deviceProp, 0);

		nameGPU = deviceProp.name;
		warpSize = deviceProp.warpSize;
		maxThreadsPerBlock = deviceProp.maxThreadsPerBlock;
		maxThreadsDim[0] = deviceProp.maxThreadsDim[0];
		maxThreadsDim[1] = deviceProp.maxThreadsDim[1];
		maxThreadsDim[2] = deviceProp.maxThreadsDim[2];
		maxGridSize[0] = deviceProp.maxGridSize[0];
		maxGridSize[1] = deviceProp.maxGridSize[1];
		maxGridSize[2] = deviceProp.maxGridSize[2];
		maxThreadsPerMultiProcessor = deviceProp.maxThreadsPerMultiProcessor;
		maxBlocksPerMultiProcessor = deviceProp.maxBlocksPerMultiProcessor;

		return true;
	}
}

using namespace Cuda;

__global__ void gravity_cuda_link_stub_kernel()
{
}

bool CudaWrapper::Init(std::string& info)
{
	if (!GetProperty()) {
		return false;
	}

	return true;
}

int CudaWrapper::Process()
{
	gravity_cuda_link_stub_kernel <<<1,1>>>();
	return true;
}

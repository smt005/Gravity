// ◦ Xyz ◦

#include "Cuda.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace Cuda;

namespace {
	void GetProperty() {
		if (deviceCount == -1) {
			cudaGetDeviceCount(&deviceCount);

			if (deviceCount == 0) {
				return;
			}

			cudaDeviceProp deviceProp;
			cudaGetDeviceProperties(&deviceProp, 0);

			nameGPU = deviceProp.name;
			warpSize = deviceProp.warpSize;
			maxThreadsPerBlock = deviceProp.maxThreadsPerBlock;
			maxThreadsDim[0] = deviceProp.maxThreadsDim[0];
			maxThreadsDim[1] = deviceProp.maxThreadsDim[1];
			maxThreadsDim[2] = deviceProp.maxThreadsDim[2];
			maxThreadsDim[3] = deviceProp.maxThreadsDim[3];
			maxGridSize[0] = deviceProp.maxGridSize[0];
			maxGridSize[1] = deviceProp.maxGridSize[1];
			maxGridSize[2] = deviceProp.maxGridSize[2];
			maxGridSize[3] = deviceProp.maxGridSize[3];
			maxThreadsPerMultiProcessor = deviceProp.maxThreadsPerMultiProcessor;
			maxBlocksPerMultiProcessor = deviceProp.maxBlocksPerMultiProcessor;
		}
	}
}

void CudaManager::PrintInfo()
{
	GetProperty();
	LOG("[CudaManager::PrintInfo] CUDA: nameGPU: {} ...", nameGPU);
}

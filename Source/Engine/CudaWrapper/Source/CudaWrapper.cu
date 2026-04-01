// ◦ Xyz ◦

#include "CudaWrapper.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace {
	void GetProperty() {
		if (Cuda::CudaWrapper::deviceCount == -1) {
			cudaGetDeviceCount(&Cuda::CudaWrapper::deviceCount);

			if (Cuda::CudaWrapper::deviceCount == 0) {
				return;
			}

			cudaDeviceProp deviceProp;
			cudaGetDeviceProperties(&deviceProp, 0);

			Cuda::CudaWrapper::nameGPU = deviceProp.name;
			Cuda::CudaWrapper::warpSize = deviceProp.warpSize;
			Cuda::CudaWrapper::maxThreadsPerBlock = deviceProp.maxThreadsPerBlock;
			Cuda::CudaWrapper::maxThreadsDim[0] = deviceProp.maxThreadsDim[0];
			Cuda::CudaWrapper::maxThreadsDim[1] = deviceProp.maxThreadsDim[1];
			Cuda::CudaWrapper::maxThreadsDim[2] = deviceProp.maxThreadsDim[2];
			Cuda::CudaWrapper::maxGridSize[0] = deviceProp.maxGridSize[0];
			Cuda::CudaWrapper::maxGridSize[1] = deviceProp.maxGridSize[1];
			Cuda::CudaWrapper::maxGridSize[2] = deviceProp.maxGridSize[2];
			Cuda::CudaWrapper::maxThreadsPerMultiProcessor = deviceProp.maxThreadsPerMultiProcessor;
			Cuda::CudaWrapper::maxBlocksPerMultiProcessor = deviceProp.maxBlocksPerMultiProcessor;
		}
	}
}

using namespace Cuda;

bool CudaWrapper::processGPU;
bool CudaWrapper::multithread;

std::string CudaWrapper::nameGPU;
int         CudaWrapper::deviceCount = -1;
int         CudaWrapper::warpSize = 0;                    // Warp size in threads
int         CudaWrapper::maxThreadsPerBlock = 0;          // Maximum number of threads per block
int         CudaWrapper::maxThreadsDim[3];                // Maximum size of each dimension of a block
int         CudaWrapper::maxGridSize[3];                  // Maximum size of each dimension of a grid
int         CudaWrapper::maxThreadsPerMultiProcessor = 0; // Maximum resident threads per multiprocessor
int         CudaWrapper::maxBlocksPerMultiProcessor = 0;


__global__ void gravity_cuda_link_stub_kernel(void)
{
}

void Cuda::CudaWrapper::PrintInfo()
{
	GetProperty();
	//LOG("[CudaWrapper::PrintInfo] CUDA: nameGPU: {} ...", Cuda::CudaWrapper::nameGPU);

	gravity_cuda_link_stub_kernel <<<1,1>>> ();
}

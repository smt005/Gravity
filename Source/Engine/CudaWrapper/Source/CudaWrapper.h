// ◦ Xyz ◦
#pragma once

#include <string>

namespace Cuda
{
	class CudaWrapper final {
	public:
		static bool Init(std::string& info);
		static int Process();

	public:
		static bool processGPU;
		static bool multithread;

		static std::string nameGPU;
		static int         deviceCount;
		static int         warpSize;                    // Warp size in threads
		static int         maxThreadsPerBlock;          // Maximum number of threads per block
		static int         maxThreadsDim[3];                // Maximum size of each dimension of a block
		static int         maxGridSize[3];                  // Maximum size of each dimension of a grid
		static int         maxThreadsPerMultiProcessor; // Maximum resident threads per multiprocessor
		static int         maxBlocksPerMultiProcessor;  // Maximum number of resident blocks per multiprocessor
	};
}

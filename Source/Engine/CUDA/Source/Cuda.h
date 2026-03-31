// ◦ Xyz ◦
#pragma once

#include <string>

namespace Cuda
{
	class CudaManager final {
	public:
		static void PrintInfo();
	
	public:
		inline static bool processGPU;
		inline static bool multithread;

		inline static std::string nameGPU;
		inline static int         deviceCount = -1;
		inline static int         warpSize = 0;                    // Warp size in threads
		inline static int         maxThreadsPerBlock = 0;          // Maximum number of threads per block
		inline static int         maxThreadsDim[3];                // Maximum size of each dimension of a block
		inline static int         maxGridSize[3];                  // Maximum size of each dimension of a grid
		inline static int         maxThreadsPerMultiProcessor = 0; // Maximum resident threads per multiprocessor
		inline static int         maxBlocksPerMultiProcessor = 0;  // Maximum number of resident blocks per multiprocessor
	};
}

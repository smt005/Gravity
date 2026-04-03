// ◦ Xyz ◦

#include "CudaWrapper.h"
#include <Logs.h>

using namespace Cuda;

bool CudaWrapper::Init(std::string& info)
{
	LOG("[CudaWrapper::Init] CUDA disabled");
	return false;
}

// ◦ Xyz ◦
#pragma once

#include "CudaClasses.h"

namespace Cuda
{
	class CudaWrapper final {
	public:
		static void Calculate(VectorWrapper<Body>& bodies, ValueWrapper<float>& ans);
	};
}

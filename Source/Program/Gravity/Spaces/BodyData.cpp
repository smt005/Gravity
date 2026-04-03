// ◦ Xyz ◦

#include "BodyData.h"
#include <iostream>
#include <Logs.h>

std::vector<BodyData> BodyData::Copy(const std::vector<Cuda::Body>& bodies)
{
	return std::vector<BodyData>(bodies.begin(), bodies.end());
}

std::ostream& operator << (std::ostream& os, const BodyData& body)
{
	os << '{' << body.mass << '_' << body.pos << '_' << body.velocity << '}';
	return os;
}

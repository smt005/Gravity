// ◦ Xyz ◦
#pragma once

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace glm
{
	std::ostream& operator<<(std::ostream& os, const mat4x4& m);
	std::ostream& operator<<(std::ostream& os, const vec4& v);
	std::ostream& operator<<(std::ostream& os, const vec3& v);
	std::ostream& operator<<(std::ostream& os, const vec2& v);
}

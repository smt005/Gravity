// ◦ Xyz ◦
#pragma once

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace glm
{
	std::ostream& operator<<(std::ostream& os, mat4x4 const& m);
	std::ostream& operator<<(std::ostream& os, vec4 const& v);
	std::ostream& operator<<(std::ostream& os, vec3 const& v);
	std::ostream& operator<<(std::ostream& os, vec2 const& v);
}

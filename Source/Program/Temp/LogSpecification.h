// ◦ Xyz ◦
#pragma once

#include <iostream>
#include <sstream>
#include <Object/Mesh.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

std::ostream& operator << (std::ostream& os, const glm::mat4x4& vec);
std::ostream& operator << (std::ostream& os, const glm::vec4& vec);
std::ostream& operator << (std::ostream& os, const glm::vec3& vec);
std::ostream& operator << (std::ostream& os, const glm::vec2& vec);

//std::ostream& operator << (std::ostream& os, const Engine::Mesh& mesh);

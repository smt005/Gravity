// ◦ Xyz ◦
#pragma once

// Log
#include <iostream>
#include <sstream>

namespace Engine
{
	class Mesh final
	{
	public:
		Mesh() = default;
		Mesh(Mesh&&) noexcept;
		void operator = (Mesh&&) noexcept;
		Mesh(Mesh&) = delete;
		void operator = (Mesh&) = delete;
		~Mesh();

		const float* Data() const;
		int CountVertexes() const;
		int SizeData() const;
		unsigned int Vbo() const;
		unsigned int Vao() const;
		void BindBuffers();

	private:
	public:
		float* vertexes = nullptr;
		float* normals = nullptr;
		float* textures = nullptr;
		int _count = 0;
		unsigned int _VBO = 0;
		unsigned int _VAO = 0;
	};
}

std::ostream& operator << (std::ostream& os, const Engine::Mesh& mesh);

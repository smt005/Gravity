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
		Mesh(int count, float* vertexes, float* normals, float* texCoords);
		void operator = (Mesh&&) noexcept;
		Mesh(Mesh&) = delete;
		void operator = (Mesh&) = delete;
		~Mesh();

		const float* Vertexes() const;
		const float* Normals() const;
		const float* TexCoords() const;
		int Count() const;
		unsigned int Vao() const;
		unsigned int Vbo() const;
		unsigned int Tbo() const;
		void BindBuffers();

	private:
		int _count = 0;
		float* _vertexes = nullptr;
		float* _normals = nullptr;
		float* _texCoords = nullptr;

		// TODO: Вынести из класса?
	public:
		unsigned int _VAO = 0;
		unsigned int _VBO = 0;
		unsigned int _TBO = 0;
		//unsigned int _NBO = 0;

	};
}

std::ostream& operator << (std::ostream& os, const Engine::Mesh& mesh);

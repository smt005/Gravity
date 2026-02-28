// ◦ Xyz ◦
#pragma once

namespace Engine
{
	class Mesh final
	{
	public:
		Mesh() = default;
		Mesh(bool load, bool bindBuffers);
		~Mesh();

		const float* Data() const;
		int Count() const;
		unsigned int Vbo() const;
		unsigned int Vao() const;
		bool Load(bool bindBuffers = false);
		void BindBuffers();

	private:
	public:
		float* _vertices = nullptr;
		int _count = 0;
		unsigned int _VBO = 0;
		unsigned int _VAO = 0;
	};
}


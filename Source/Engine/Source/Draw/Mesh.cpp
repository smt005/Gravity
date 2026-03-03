// ◦ Xyz ◦

#include "Mesh.h"
#include <glad/gl.h>

using namespace Engine;

Mesh::Mesh(Mesh&& mesh) noexcept
{
	std::swap(*this, mesh);
}

void Mesh::operator = (Mesh&& mesh) noexcept
{
	std::swap(vertexes, mesh.vertexes);
	std::swap(normals, mesh.normals);
	std::swap(textures, mesh.textures);
	std::swap(_count, mesh._count);
	std::swap(_VBO, mesh._VBO);
	std::swap(_VAO, mesh._VAO);
}

Mesh::~Mesh()
{


    // TODO: Удалить _VBO, _VAO
}

const float* Mesh::Data() const
{
	return vertexes;
}

int Mesh::CountVertexes() const
{
	return _count;
}

int Mesh::SizeData() const
{
    return _count * 3;
}

unsigned int Mesh::Vbo() const
{
    return _VBO;
}

unsigned int Mesh::Vao() const
{
    return _VAO;
}

void Mesh::BindBuffers()
{
    if (SizeData() <= 0 || vertexes == nullptr) {
        return;
    }

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, SizeData() * sizeof(float), vertexes, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // layout location
        3,                  // vec3
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

std::ostream& operator << (std::ostream& os, const Engine::Mesh& mesh)
{
	{
		const size_t size = mesh.CountVertexes();
		os << "VERTEX [";

		for (int i = 0; i < size; ++i) {
			try {
				float value = mesh.Data()[i];
				os << value << ',';
			}
			catch (...) {
				os << "VERTEX STOP i: " << i << " / " << size;
			}
		}

		os << "]\n";
	}
	{
		const size_t size = mesh.SizeData();
		os << "DATA [";

		for (int i = 0; i < size; ++i) {
			try {
				float value = mesh.Data()[i];
				os << value << ',';
			}
			catch (...) {
				os << "DATA STOP i: " << i << " / " << size;
			}
		}

		os << "]\n";
	}

	return os;
}
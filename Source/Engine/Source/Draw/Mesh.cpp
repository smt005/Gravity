// ◦ Xyz ◦

#include "Mesh.h"
#include <glad/gl.h>

using namespace Engine;

Mesh::Mesh(Mesh&& mesh) noexcept
	: _count(mesh._count)
	, _vertexes(mesh._vertexes)
	, _normals(mesh._normals)
	, _texCoords(mesh._texCoords)
	, _VBO(mesh._VBO)
	, _VAO(mesh._VAO)
{
	mesh._count = 0;
	mesh._vertexes = nullptr;
	mesh._normals = nullptr;
	mesh._texCoords = nullptr;
	mesh._VBO = 0;
	mesh._VAO = 0;
}

Mesh::Mesh(int count, float* vertexes, float* normals, float* texCoords)
	: _count(count)
	, _vertexes(vertexes)
	, _normals(normals)
	, _texCoords(texCoords)
{}

void Mesh::operator = (Mesh&& mesh) noexcept
{
	std::swap(_count, mesh._count);
	std::swap(_vertexes, mesh._vertexes);
	std::swap(_normals, mesh._normals);
	std::swap(_texCoords, mesh._texCoords);
	std::swap(_VBO, mesh._VBO);
	std::swap(_VAO, mesh._VAO);
}

Mesh::~Mesh()
{
	delete[] _vertexes;
	delete[] _normals;
	delete[] _texCoords;

    // TODO: Удалить _VBO, _VAO
}

const float* Mesh::Vertexes() const
{
	return _vertexes;
}

const float* Mesh::Normals() const
{
	return _vertexes;
}

const float* Mesh::TexCoords() const
{
	return _vertexes;
}

int Mesh::Count() const
{
	return _count;
}

unsigned int Mesh::Vbo() const
{
    return _VBO;
}

/*unsigned int Mesh::Vao() const
{
    return _VAO;
}*/

unsigned int Mesh::Vao() const {
	return _VAO;
}

void Mesh::BindBuffers()
{
    if (_count <= 0 || _vertexes == nullptr) {
        return;
    }

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _count * 3 * sizeof(float), _vertexes, GL_STATIC_DRAW);

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
	const size_t size = mesh.Count() * 3;
	os << "VERTEX [" << size << "]:\n[";

	for (int i = 0; i < size; i += 3) {
		// TODO: Проверка на выход за пределы массива
		os << '{' << mesh.Vertexes()[i] << ',' << mesh.Vertexes()[i + 1] << ',' << mesh.Vertexes()[i + 2] << '}';
		if (i + 3 < size) {
			os << ',';
		}
	}

	os << "]\n";
	return os;
}

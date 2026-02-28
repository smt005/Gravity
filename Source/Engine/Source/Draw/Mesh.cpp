// ◦ Xyz ◦

#include "Mesh.h"
#include <glad/gl.h>

using namespace Engine;

Mesh::Mesh(bool load, bool bindBuffers)
{
    if (load) {
        Load(bindBuffers);
    }
}

Mesh::~Mesh()
{
	delete[] _vertices;

    // TODO: Удалить _VBO, _VAO
}

const float* Mesh::Data() const
{
	return _vertices;
}

int Mesh::Count() const
{
	return _count;
}

unsigned int Mesh::Vbo() const
{
    return _VBO;
}

unsigned int Mesh::Vao() const
{
    return _VAO;
}

bool Mesh::Load(bool bindBuffers)
{
    delete[] _vertices;
    _vertices = nullptr;

    _vertices = new float[] {
        // Передняя грань
        -0.1f,  0.1f,  0.1f,
        -0.1f, -0.1f,  0.1f,
         0.1f, -0.1f,  0.1f,

        -0.1f,  0.1f,  0.1f,
         0.1f, -0.1f,  0.1f,
         0.1f,  0.1f,  0.1f,

         // Задняя грань
         -0.1f,  0.1f, -0.1f,
         -0.1f, -0.1f, -0.1f,
          0.1f, -0.1f, -0.1f,

         -0.1f,  0.1f, -0.1f,
          0.1f, -0.1f, -0.1f,
          0.1f,  0.1f, -0.1f,

          // Боковые грани (каждый набор — два треугольника)
          // Левая боковая грань
          -0.1f,  0.1f,  0.1f,
          -0.1f, -0.1f,  0.1f,
          -0.1f, -0.1f, -0.1f,

          -0.1f,  0.1f,  0.1f,
          -0.1f, -0.1f, -0.1f,
          -0.1f,  0.1f, -0.1f,

          // Правая боковая грань
           0.1f,  0.1f,  0.1f,
           0.1f, -0.1f,  0.1f,
           0.1f, -0.1f, -0.1f,

           0.1f,  0.1f,  0.1f,
           0.1f, -0.1f, -0.1f,
           0.1f,  0.1f, -0.1f,

           // Верхняя боковая грань
           -0.1f,  0.1f,  0.1f,
            0.1f,  0.1f,  0.1f,
            0.1f,  0.1f, -0.1f,

           -0.1f,  0.1f,  0.1f,
            0.1f,  0.1f, -0.1f,
           -0.1f,  0.1f, -0.1f,

           // Нижняя боковая грань
           -0.1f, -0.1f,  0.1f,
            0.1f, -0.1f,  0.1f,
            0.1f, -0.1f, -0.1f,

           -0.1f, -0.1f,  0.1f,
            0.1f, -0.1f, -0.1f,
           -0.1f, -0.1f, -0.1f
    };

    _count = 108;// 36;

    if (bindBuffers) {
        BindBuffers();
    }

    return true;
}

void Mesh::BindBuffers()
{
    if (_count <= 0 || _vertices == nullptr) {
        return;
    }

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _count * sizeof(float), _vertices, GL_STATIC_DRAW);

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

#include "Plane.h"
#include <vector>

struct PlaneVertex
{
    GLfloat x,y,z;
    GLfloat u, v;
};

GLuint Plane::s_program = 0;
int Plane::s_refCount = 0;

Plane::Plane(int subdivisions)
{
    if(subdivisions < 0)
        subdivisions = 0;
    if(subdivisions > 10)
        subdivisions = 10;

    if(s_refCount == 0)
    {
        s_program = wolf::LoadShaders("data/plane.vsh", "data/plane.fsh");
        if(!s_program)
            printf("Plane failed to load shaders \n");
    }
    ++s_refCount;

    _buildMesh(subdivisions);
};

Plane::~Plane()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);

    --s_refCount;
    if(s_refCount == 0)
    {
        glDeleteProgram(s_program);
        s_program = 0;
    }
}

void Plane::_buildMesh(int subdivisions)
{
    const int n = 1 << subdivisions; // quads per side
    const int vertsPerSide = n+1; // one more vertices than quads

    std::vector<PlaneVertex> verts;
    verts.reserve(vertsPerSide * vertsPerSide);

    for(int row = 0; row < vertsPerSide; ++row)
    {
        for(int col = 0; col < vertsPerSide; ++col)
        {
            PlaneVertex v;
            v.x = -0.5f + (float)col / (float)n;
            v.y = 0.0f;
            v.z = -0.5f + (float)row / (float)n;
            verts.push_back(v);
        }
    }

    std::vector<unsigned int> indices;
    indices.reserve(n * n * 6);
    for(int row = 0; row < n; ++row)
    {
        for(int col = 0; col < n; ++col)
        {
            unsigned int i0 = row* vertsPerSide + col;
            unsigned int i1 = i0 + 1;
            unsigned int i2 = i0 + vertsPerSide;
            unsigned int i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }
    m_numIndices = (int)indices.size();

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(PlaneVertex) * verts.size(),
        verts.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned int) * indices.size(),
        indices.data(),
        GL_STATIC_DRAW
    );

    int posAttr = glGetAttribLocation(s_program,"a_position");
    if(posAttr >= 0)
    {
        glVertexAttribPointer(
            posAttr,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(PlaneVertex),
            0
        );
        glEnableVertexAttribArray(posAttr);
    }
    glBindVertexArray(0);
}

void Plane::render(const glm::mat4& view, const glm::mat4& proj)
{
    glm::mat4 world = glm::translate(glm::mat4(1.0f), m_position);
    world = glm::scale(world, m_scale);

    glm::mat4 transformed = proj * view * world;

    glUseProgram(s_program);
    glUniformMatrix4fv(
        glGetUniformLocation(s_program, "transformed"),
        1,
        GL_FALSE,
        glm::value_ptr(transformed)
    );
    glUniform4fv(
        glGetUniformLocation(s_program, "u_color"),
        1,
        glm::value_ptr(m_color)
    );
    
    glBindVertexArray(m_vao);
    glDrawElements(
        GL_TRIANGLES,
        m_numIndices,
        GL_UNSIGNED_INT,
        0
    );
}
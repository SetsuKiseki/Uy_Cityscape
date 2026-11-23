#pragma once
#include "../wolf/wolf.h"

// I might use this to make terrain changes so that's why I'm using the plane code.
class Plane
{
public:
    Plane(int subdivisions = 0);
    ~Plane();

    Plane(const Plane&) = delete;
    Plane& operator = (const Plane&) = delete;

    void setPosition(const glm::vec3& position) {m_position = position;} // x, y, z
    void setScale(const glm::vec3& scale) {m_scale = scale;} // x, y, z
    void setColor(float r, float g, float b, float a = 1.0f) {m_color = glm::vec4(r, g, b, a);}
    void render(const glm::mat4& view, const glm::mat4& proj);

private:
    void _buildMesh(int subdivision);

    static GLuint s_program;
    static int s_refCount;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ibo = 0;
    int m_numIndices = 0;

    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    glm::vec4 m_color = glm::vec4(0.247f, 0.608f, 0.043f, 1.0f);
};
#pragma once
#include "../wolf/wolf.h"

class Cube
{
    public:
        Cube();
        ~Cube();

        // Transform methods
        void setPosition(const glm::vec3& pos) {m_position = pos;}
        void setScale(const glm::vec3& scale) {m_scale = scale;}
        void setScale(float uniformScale) {m_scale = glm::vec3(uniformScale);}
        void setRotation(const glm::vec3& radians) {m_rotation = radians;} // x,y,z angles in radians
        void rotate(const glm::vec3& deltaRadians) {m_rotation += deltaRadians;}

        // Color methods
        void setColor(const glm::vec4& color) {m_color = color;}
        void setColor(float r, float g, float b, float a = 1.0f) {m_color = glm::vec4(r,g,b,a);}

        // Getters
        glm::vec3 getPosition() const {return m_position;}
        glm::vec3 getScale() const {return m_scale;}
        glm::vec3 getRotation() const {return m_rotation;}
        glm::vec4 getColor() const {return m_color;}

        void render(const glm::mat4& view, const glm::mat4& proj);

    private:
        glm::mat4 getWorldMatrix() const;

        // Variables for individual cube instance
        glm::vec3 m_position = glm::vec3(0.0f);
        glm::vec3 m_scale = glm::vec3(1.0f);
        glm::vec3 m_rotation = glm::vec3(0.0f);
        glm::vec4 m_color = glm::vec4(1.0f);

        // Shared resources for all Cube instances
        static GLuint s_program;
        static GLuint s_vbo;
        static GLuint s_ibo;
        static GLuint s_vao;
        static int s_refCount;
    };
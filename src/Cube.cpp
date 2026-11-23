#include "Cube.h"

struct CubeVertex
{
    GLfloat x, y, z; // position
    GLfloat nx, ny, nz; // face
    GLfloat u, v; // texture coordinates
    GLubyte r, g, b, a; // shade
};

static const unsigned short gs_cubeIndices [] = {
    0, 1, 2,        0, 2, 3,    //Front
    4, 5, 6,        4, 6, 7,    //Back
    8, 9, 10,       8, 10, 11,  //Left
    12, 13, 14,     12, 14, 15, //Right
    16, 17, 18,     16, 18, 19, //Top
    20, 21, 22,     20, 22, 23  //Bottom
};

static const CubeVertex gs_cubeVertices[] = {
    // Front (+z)
    { -0.5f, -0.5f,  0.5f, 255, 255, 255, 255 },
    {  0.5f, -0.5f,  0.5f, 255, 255, 255, 255 },
    {  0.5f,  0.5f,  0.5f, 255, 255, 255, 255 },
    { -0.5f,  0.5f,  0.5f, 255, 255, 255, 255 },

    // Back (-z)
    {  0.5f, -0.5f, -0.5f, 140, 140, 140, 255 },
    { -0.5f, -0.5f, -0.5f, 140, 140, 140, 255 },
    { -0.5f,  0.5f, -0.5f, 140, 140, 140, 255 },
    {  0.5f,  0.5f, -0.5f, 140, 140, 140, 255 },

    // Left (-x)
    { -0.5f, -0.5f, -0.5f, 200, 200, 200, 255 },
    { -0.5f, -0.5f,  0.5f, 200, 200, 200, 255 },
    { -0.5f,  0.5f,  0.5f, 200, 200, 200, 255 },
    { -0.5f,  0.5f, -0.5f, 200, 200, 200, 255 },

    // Right (+x)
    {  0.5f, -0.5f,  0.5f, 170, 170, 170, 255 },
    {  0.5f, -0.5f, -0.5f, 170, 170, 170, 255 },
    {  0.5f,  0.5f, -0.5f, 170, 170, 170, 255 },
    {  0.5f,  0.5f,  0.5f, 170, 170, 170, 255 },

    // Top (+y)
    { -0.5f,  0.5f,  0.5f, 230, 230, 230, 255 },
    {  0.5f,  0.5f,  0.5f, 230, 230, 230, 255 },
    {  0.5f,  0.5f, -0.5f, 230, 230, 230, 255 },
    { -0.5f,  0.5f, -0.5f, 230, 230, 230, 255 },

    // Bottom (-y)
    { -0.5f, -0.5f, -0.5f, 110, 110, 110, 255 },
    {  0.5f, -0.5f, -0.5f, 110, 110, 110, 255 },
    {  0.5f, -0.5f,  0.5f, 110, 110, 110, 255 },
    { -0.5f, -0.5f,  0.5f, 110, 110, 110, 255 },
};

static const int CUBE_NUM_INDICES = sizeof(gs_cubeIndices) /sizeof(gs_cubeIndices[0]); //36 indices
static const int CUBE_NUM_VERTS = sizeof(gs_cubeVertices) / sizeof(gs_cubeVertices[0]); //24 vertices

// Uniform locations looked up only once instead of every draw name
static GLint s_locTransformed = -1;
static GLint s_locColor = -1;
static GLint s_locScale = -1;
static GLint s_locTileSize = -1;

GLuint Cube::s_program = 0;
GLuint Cube::s_vbo = 0;
GLuint Cube::s_ibo = 0;
GLuint Cube::s_vao = 0;
int Cube::s_refCount = 0;

Cube::Cube()
{
    if(s_refCount == 0)
    {
        // Create the shader program
        s_program = wolf::LoadShaders("data/cubeColor.vsh", "data/cube.fsh");

        if(!s_program)
            printf("Cube failed to load shaders\n");
        else
        {
            s_locTransformed = glGetUniformLocation(s_program, "transformed");
            s_locColor = glGetUniformLocation(s_program, "u_color");
            s_locScale = glGetUniformLocation(s_program, "u_scale");
            s_locTileSize = glGetUniformLocation(s_program, "u_tileSize");

            glUseProgram(s_program);
            glUniform1i(glGetUniformLocation(s_program,"tex"), 0);
        }

        // Create VBO and VAO
        glGenVertexArrays(1, &s_vao);
        glBindVertexArray(s_vao);

        // Vertex Buffer
        glGenBuffers(1, &s_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gs_cubeVertices), gs_cubeVertices, GL_STATIC_DRAW);

        // Index Buffer
        glGenBuffers(1, &s_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gs_cubeIndices), gs_cubeIndices, GL_STATIC_DRAW);

        int posAttr = glGetAttribLocation(s_program, "a_position");
        int normalAttr = glGetAttribLocation(s_program, "a_normal");
        int uvAttr = glGetAttribLocation(s_program, "a_uv");
        int colorAttr = glGetAttribLocation(s_program, "a_color");

        if (posAttr >= 0)
        {
            glVertexAttribPointer(
                                posAttr,                // Attribute location
                                3,                      // Number of components
                                GL_FLOAT,               // Type of each component
                                GL_FALSE,               // Normalize?
                                sizeof(CubeVertex),     // Stride
                                0);                     // Offset
            glEnableVertexAttribArray(posAttr);
        }
        if (normalAttr >= 0)
        {
            glVertexAttribPointer(
                                normalAttr,                     // Attribute location
                                3,                              // Number of components
                                GL_FLOAT,                       // Type of each component
                                GL_FALSE,                       // Normalize?
                                sizeof(CubeVertex),             // Stride
                                (void*)offsetof(CubeVertex, nx) // Offset
            );
            glEnableVertexAttribArray(normalAttr);
        }
        if (uvAttr >= 0)
        {
            glVertexAttribPointer(
                                uvAttr,                         // Attribute location
                                2,                              // Number of components
                                GL_FLOAT,                       // Type of each component
                                GL_FALSE,                       // Normalize?
                                sizeof(CubeVertex),             // Stride
                                (void*)offsetof(CubeVertex, u)  // Offset
            );
            glEnableVertexAttribArray(uvAttr);
        }
        if (colorAttr >= 0)
        {
            glVertexAttribPointer(
                                colorAttr,                      // Attribute location
                                4,                              // Number of components
                                GL_UNSIGNED_BYTE,               // Type of each component
                                GL_TRUE,                        // Normalize?
                                sizeof(CubeVertex),             // Stride
                                (void *)(sizeof(GLfloat) * 3)); // Offset
            glEnableVertexAttribArray(colorAttr);
        }

        glBindVertexArray(0); // removes VAO from being bound so it can't be changed by accident
    }
    ++s_refCount;
    printf ("Successfully created Cube instance. Current ref count: %d\n", s_refCount);
}

Cube::~Cube()
{
    --s_refCount;
    if(s_refCount == 0)
    {
        glDeleteVertexArrays(1, &s_vao);
        glDeleteBuffers(1, &s_vbo);
        glDeleteBuffers(1, &s_ibo);
        glDeleteProgram(s_program);
        s_vao = s_vbo = s_ibo = s_program = 0;
        s_locTransformed = s_locColor = s_locScale = s_locTileSize = -1;
    }
}

glm::mat4 Cube::getWorldMatrix() const
{
    glm::mat4 world = glm::translate(
        glm::mat4(1.0f),
        m_position
    );
    world = glm::rotate(
        world,
        m_rotation.x,
        glm::vec3(1.0f, 0.0f, 0.0f));
    world = glm::rotate(
        world,
        m_rotation.y,
        glm::vec3(0.0f, 1.0f, 0.0f));
    world = glm::rotate(
        world,
        m_rotation.z,
        glm::vec3(0.0f, 0.0f, 1.0f));
    world = glm::scale(
        world, m_scale);
    return world;
}

void Cube::render(const glm::mat4& view, const glm::mat4& proj)
{
    // Use shader program.
    glUseProgram(s_program);

    glm::mat4 transformed = proj * view * getWorldMatrix();

    // set uniform
    glUniformMatrix4fv(
        s_locTransformed, 
        1,
        GL_FALSE,
        glm::value_ptr(transformed)
    );
    glUniform4fv(
        s_locColor, 
        1,
        glm::value_ptr(m_color)
    );
    // Sizes the UV per face
    glUniform3fv(
        s_locScale, 
        1,
        glm::value_ptr(m_color)
    );
    glUniform1f(s_locTileSize, m_tileSize);
    

    // Bind VAO and draw
    glBindVertexArray(s_vao);
    glDrawElements(GL_TRIANGLES, CUBE_NUM_INDICES, GL_UNSIGNED_SHORT, 0);
}
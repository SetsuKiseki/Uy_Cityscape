#include "sampleCube.h"
#include "../samplefw/Grid2D.h"

struct Vertex
{
	GLfloat x,y,z;
	GLubyte r,g,b,a;
};

static const Vertex gs_cubeVertices[] = {
// Front
	{ -0.5f, -0.5f, 0.5f, 255, 0, 0, 255 },
	{ -0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
	{  0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
	{  0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
	{  0.5f, -0.5f, 0.5f, 255, 0, 0, 255 },
	{ -0.5f, -0.5f, 0.5f, 255, 0, 0, 255 },

	// Back
	{  0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },
	{ -0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },
	{ -0.5f, -0.5f,-0.5f, 128, 0, 0, 255 },
	{ -0.5f, -0.5f,-0.5f, 128, 0, 0, 255 },
	{  0.5f, -0.5f,-0.5f, 128, 0, 0, 255 },
	{  0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },

	// Left
	{ -0.5f,  0.5f,-0.5f, 0, 255, 0, 255 },
	{ -0.5f,  0.5f, 0.5f, 0, 255, 0, 255 },
	{ -0.5f, -0.5f, 0.5f, 0, 255, 0, 255 },
	{ -0.5f, -0.5f, 0.5f, 0, 255, 0, 255 },
	{ -0.5f, -0.5f,-0.5f, 0, 255, 0, 255 },
	{ -0.5f,  0.5f,-0.5f, 0, 255, 0, 255 },

	// Right
	{  0.5f,  0.5f, 0.5f, 0, 128, 0, 255 },
	{  0.5f,  0.5f,-0.5f, 0, 128, 0, 255 },
	{  0.5f, -0.5f,-0.5f, 0, 128, 0, 255 },
	{  0.5f, -0.5f,-0.5f, 0, 128, 0, 255 },
	{  0.5f, -0.5f, 0.5f, 0, 128, 0, 255 },
	{  0.5f,  0.5f, 0.5f, 0, 128, 0, 255 },

	// Top
	{ -0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },
	{ -0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
	{  0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
	{  0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
	{  0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },
	{ -0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },

	// Bottom
	{ -0.5f, -0.5f, 0.5f, 0, 0, 128, 255 },
	{  0.5f, -0.5f, 0.5f, 0, 0, 128, 255 },
	{  0.5f, -0.5f,-0.5f, 0, 0, 128, 255 },
	{  0.5f, -0.5f,-0.5f, 0, 0, 128, 255 },
	{ -0.5f, -0.5f,-0.5f, 0, 0, 128, 255 },
	{ -0.5f, -0.5f, 0.5f, 0, 0, 128, 255 },
};

SampleCube::~SampleCube()
{
	printf("Destroying Cube Sample\n");
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteProgram(m_program);
}

void SampleCube::init()
{
    // Only init if not already done
    if(!m_program)
    {
        glEnable(GL_DEPTH_TEST);

        m_program = wolf::LoadShaders("data/cube.vsh", "data/cube.fsh");

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6 * 3 * 2, gs_cubeVertices, GL_STATIC_DRAW);

        // Set attribute pointers
        int posAttr = glGetAttribLocation(m_program, "a_position");
        int colorAttr = glGetAttribLocation(m_program, "a_color");

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexAttribPointer(posAttr,       // Attribute location
                              3,              // Number of components
                              GL_FLOAT,       // Type of each component
                              GL_FALSE,       // Normalize?
                              sizeof(Vertex), // Stride
                              0);             // Offset
        glEnableVertexAttribArray(posAttr);

        glVertexAttribPointer(colorAttr,                       // Attribute location
                              4,                              // Number of components
                              GL_UNSIGNED_BYTE,               // Type of each component
                              GL_TRUE,                        // Normalize?
                              sizeof(Vertex),                 // Stride
                              (void *)(sizeof(GLfloat) * 3)); // Offset
        glEnableVertexAttribArray(colorAttr);
    }

    printf("Successfully initialized Cube Sample\n");
}

void SampleCube::update(float dt) 
{
	m_rot += dt;
}

void SampleCube::render(int width, int height)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader program.
    glUseProgram(m_program);
    
	glm::mat4 mProj = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f);
	glm::mat4 mView = glm::lookAt(glm::vec3(0.0f,0.0f,2.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 mWorld = glm::rotate(glm::mat4(1.0f), m_rot, glm::vec3(0.0f, 1.0f, 0.0f));
	mWorld = mWorld * glm::rotate(glm::mat4(1.0f), m_rot, glm::vec3(1.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(m_program,"projection"), 1, GL_FALSE, glm::value_ptr(mProj));
	glUniformMatrix4fv(glGetUniformLocation(m_program,"view"), 1, GL_FALSE, glm::value_ptr(mView));
	glUniformMatrix4fv(glGetUniformLocation(m_program,"world"), 1, GL_FALSE, glm::value_ptr(mWorld));
    
	glBindVertexArray(m_vao);

    // Draw!
	glDrawArrays(GL_TRIANGLES, 0, 6 * 3 * 2);
}


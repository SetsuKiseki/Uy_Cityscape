#include "sampleInterleaved.h"
#include "../samplefw/Grid2D.h"

struct Vertex
{
	GLfloat x,y;
	GLubyte r,g,b,a;
};

static const Vertex gs_squareVertices[] = {
	{ -1.0f,  -1.0f,	255, 255, 0, 255 },
	{ 1.0f,  -1.0f,		0, 255, 0, 255 },
	{ -1.0f,   1.0f,	255, 0, 0, 255 },
	
	{ -1.0f,   1.0f,	255, 0, 0, 255 },
	{ 1.0f,  -1.0f,		0, 255, 0, 255 },
	{ 1.0f,   1.0f,		0, 255, 255, 255 },
};

SampleInterleaved::~SampleInterleaved()
{
	printf("Destroying Interleaved Example\n");
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vboVerts);
	glDeleteProgram(m_program);
}

void SampleInterleaved::init()
{
    // Only init if not already done
    if(!m_program)
    {
		m_program = wolf::LoadShaders("data/colored.vsh", "data/colored.fsh");

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		
		// Create VBO for vertices
		glGenBuffers(1, &m_vboVerts);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboVerts);
		
		glBufferData(GL_ARRAY_BUFFER, // Vertex Data
					sizeof(Vertex) * 6,  // The total size of the buffer, in bytes
					gs_squareVertices, // Pointer to the data to copy over to VRAM
					GL_STATIC_DRAW); // Hint to the driver for how it’ll be used.

		// Set attribute pointers
		int posAttr = glGetAttribLocation(m_program, "a_position");
		int colorAttr = glGetAttribLocation(m_program, "a_color");

		glBindBuffer(GL_ARRAY_BUFFER, m_vboVerts);
		glVertexAttribPointer(posAttr,					// Attribute location
							2,						// Number of components
							GL_FLOAT,					// Type of each component
							GL_FALSE,					// Normalize?
							sizeof(Vertex),			// Stride
							0);						// Offset
		glEnableVertexAttribArray(posAttr);

		glVertexAttribPointer(colorAttr,					// Attribute location
							4,						// Number of components
							GL_UNSIGNED_BYTE,			// Type of each component
							GL_TRUE,					// Normalize?
							sizeof(Vertex),			// Stride
							(void*) (sizeof(GLfloat) * 2));		// Offset
		glEnableVertexAttribArray(colorAttr);
    }

    printf("Successfully initialized Interleaved Example\n");
}

void SampleInterleaved::update(float dt) 
{
}

void SampleInterleaved::render(int width, int height)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader program.
    glUseProgram(m_program);
    
    glBindVertexArray(m_vao);
    
    // Draw!
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

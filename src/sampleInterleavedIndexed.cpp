#include "sampleInterleavedIndexed.h"
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
	{ 1.0f,   1.0f,		0, 255, 255, 255 },
};

static const unsigned short gs_squareIndices[] = {
	0,
	1,
	2,

	2,
	1,
	3,
};

SampleInterleavedIndexed::~SampleInterleavedIndexed()
{
	printf("Destroying Interleaved with Index Buffers Sample\n");
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_indexBuffer);
	glDeleteProgram(m_program);
}

void SampleInterleavedIndexed::init()
{
    // Only init if not already done
    if(!m_program)
    {
        m_program = wolf::LoadShaders("data/colored.vsh", "data/colored.fsh");

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glBufferData(GL_ARRAY_BUFFER,    // Vertex Data
                     sizeof(Vertex) * 4, // The total size of the buffer, in bytes
                     gs_squareVertices,     // Pointer to the data to copy over to VRAM
                     GL_STATIC_DRAW);    // Hint to the driver for how it’ll be used.

        glGenBuffers(1, &m_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, // Index Data
                     6 * sizeof(GLushort),    // The total size of the buffer, in bytes
                     gs_squareIndices,           // Pointer to the data to copy over to VRAM
                     GL_STATIC_DRAW);         // Hint to the driver for how it'll be used.

        // Set attribute pointers
        int posAttr = glGetAttribLocation(m_program, "a_position");
        int colorAttr = glGetAttribLocation(m_program, "a_color");

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexAttribPointer(posAttr,       // Attribute location
                              2,              // Number of components
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
                              (void *)(sizeof(GLfloat) * 2)); // Offset
        glEnableVertexAttribArray(colorAttr);
    }

    printf("Successfully initialized Interleaved with Index Buffers Sample\n");
}

void SampleInterleavedIndexed::update(float dt) 
{
}

void SampleInterleavedIndexed::render(int width, int height)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader program.
    glUseProgram(m_program);
    
	glBindVertexArray(m_vao);
    
    // Draw!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}


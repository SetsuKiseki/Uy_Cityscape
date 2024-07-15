#include "sampleMapBuffer.h"
#include "../samplefw/Grid2D.h"

struct Vertex
{
	GLfloat x,y,z;
	GLubyte r,g,b,a;
};

static const Vertex gs_cubeVertices[] = {
// Front
	{ -0.5f, 0.0f, 0.5f, 255, 0, 0, 255 },
	{ -0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
	{  0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
	{  0.5f,  0.5f, 0.5f, 255, 0, 0, 255 },
	{  0.5f, 0.0f, 0.5f, 255, 0, 0, 255 },
	{ -0.5f, 0.0f, 0.5f, 255, 0, 0, 255 },

	// Back
	{  0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },
	{ -0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },
	{ -0.5f, 0.0f,-0.5f, 128, 0, 0, 255 },
	{ -0.5f, 0.0f,-0.5f, 128, 0, 0, 255 },
	{  0.5f, 0.0f,-0.5f, 128, 0, 0, 255 },
	{  0.5f,  0.5f,-0.5f, 128, 0, 0, 255 },

	// Left
	{ -0.5f,  0.5f,-0.5f, 0, 255, 0, 255 },
	{ -0.5f,  0.5f, 0.5f, 0, 255, 0, 255 },
	{ -0.5f, 0.0f, 0.5f, 0, 255, 0, 255 },
	{ -0.5f, 0.0f, 0.5f, 0, 255, 0, 255 },
	{ -0.5f, 0.0f,-0.5f, 0, 255, 0, 255 },
	{ -0.5f,  0.5f,-0.5f, 0, 255, 0, 255 },

	// Right
	{  0.5f,  0.5f, 0.5f, 0, 128, 0, 255 },
	{  0.5f,  0.5f,-0.5f, 0, 128, 0, 255 },
	{  0.5f, 0.0f,-0.5f, 0, 128, 0, 255 },
	{  0.5f, 0.0f,-0.5f, 0, 128, 0, 255 },
	{  0.5f, 0.0f, 0.5f, 0, 128, 0, 255 },
	{  0.5f,  0.5f, 0.5f, 0, 128, 0, 255 },

	// Top
	{ -0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },
	{ -0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
	{  0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
	{  0.5f,  0.5f,-0.5f, 0, 0, 255, 255 },
	{  0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },
	{ -0.5f,  0.5f, 0.5f, 0, 0, 255, 255 },

	// Bottom
	{ -0.5f, 0.0f, 0.5f, 0, 0, 128, 255 },
	{  0.5f, 0.0f, 0.5f, 0, 0, 128, 255 },
	{  0.5f, 0.0f,-0.5f, 0, 0, 128, 255 },
	{  0.5f, 0.0f,-0.5f, 0, 0, 128, 255 },
	{ -0.5f, 0.0f,-0.5f, 0, 0, 128, 255 },
	{ -0.5f, 0.0f, 0.5f, 0, 0, 128, 255 },
};

const int VERTS_PER_CUBOID = 6 * 3 * 2;
const int CUBES_WIDE = 4;
const int CUBES_DEEP = 4;
const int NUM_VERTS = VERTS_PER_CUBOID * CUBES_WIDE * CUBES_DEEP;

void _calculateVertices(Vertex* pVerts, int cubesWide, int cubesDeep)
{
	float currX = -(float)cubesWide / 2.0f;
	float currZ = -(float)cubesDeep / 2.0f;

	for (int z = 0; z < cubesDeep; ++z)
	{
		for (int x = 0; x < cubesWide; ++x)
		{
			float height = 0.5f + 2.0f * (float)rand() / (float)RAND_MAX;
			float shade = 0.25 + 0.75 * (float)rand() / (float)RAND_MAX;

			for (int i = 0; i < VERTS_PER_CUBOID; ++i)
			{
				pVerts->x = gs_cubeVertices[i].x + currX;
				pVerts->y = gs_cubeVertices[i].y * height;
				pVerts->z = gs_cubeVertices[i].z + currZ;
				pVerts->r = (unsigned char) ((float)gs_cubeVertices[i].r * shade);
				pVerts->g = (unsigned char) ((float)gs_cubeVertices[i].g * shade);
				pVerts->b = (unsigned char) ((float)gs_cubeVertices[i].b * shade);
				pVerts->a = gs_cubeVertices[i].a;
				pVerts++;
			}

			currX += 1.5f;
		}

		currX = -(float)cubesWide / 2.0f;
		currZ += 1.5f;
	}
}

SampleMapBuffer::~SampleMapBuffer()
{
	printf("Destroying Map Buffer Sample\n");
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteProgram(m_program);
	delete m_pOrbitCam;
}

void SampleMapBuffer::init()
{
    // Only init if not already done
    if(!m_program)
    {
		// NOTE: There are much better ways to do what this demo is doing - it's just a fun
		//       way to show off glMapBuffer.
        glEnable(GL_DEPTH_TEST);

        m_program = wolf::LoadShaders("data/cube.vsh", "data/cube.fsh");

		Vertex *pVerts = new Vertex[NUM_VERTS];
		_calculateVertices(pVerts, CUBES_WIDE, CUBES_DEEP);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * NUM_VERTS, pVerts, GL_STATIC_DRAW);

		delete[] pVerts;

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
		
		m_pOrbitCam = new OrbitCamera(m_pApp);
		m_pOrbitCam->focusOn(glm::vec3(-5.0f,-5.0f,-5.0f), glm::vec3(5.0f,5.0f,5.0f));
    }

    printf("Successfully initialized Map Buffer Sample\n");
}

void SampleMapBuffer::update(float dt) 
{
	m_timer += dt;
	m_rot += dt;
	m_pOrbitCam->update(dt);

	if (m_timer > 0.05f)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		Vertex *pVerts = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		_calculateVertices(pVerts, CUBES_WIDE, CUBES_DEEP);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		m_timer = 0.0f;
	}
}

void SampleMapBuffer::render(int width, int height)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mProj = m_pOrbitCam->getProjMatrix(width,height);
	glm::mat4 mView = m_pOrbitCam->getViewMatrix();
	glm::mat4 mWorld = glm::mat4(1.0f);//glm::rotate(glm::mat4(1.0f), m_rot, glm::vec3(0.0f, 1.0f, 0.0f));
	// mWorld = mWorld * glm::rotate(glm::mat4(1.0f), m_rot, glm::vec3(1.0f, 0.0f, 0.0f));

    // Use shader program.
    glUseProgram(m_program);
    
	// Bind Uniforms
	glUniformMatrix4fv(glGetUniformLocation(m_program,"projection"), 1, GL_FALSE, glm::value_ptr(mProj));
	glUniformMatrix4fv(glGetUniformLocation(m_program,"view"), 1, GL_FALSE, glm::value_ptr(mView));
	glUniformMatrix4fv(glGetUniformLocation(m_program,"world"), 1, GL_FALSE, glm::value_ptr(mWorld));
    
	// Set up source data
	glBindVertexArray(m_vao);

    // Draw!
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS);
}



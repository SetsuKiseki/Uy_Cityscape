#include "sampleCube.h"
#include "../samplefw/Grid2D.h"

const float SPACING = 10.0f; // space between individual cubes

SampleCube::~SampleCube()
{
	printf("Destroying Cube Sample\n");
	delete m_pCubeManager;
	delete m_pOrbitCam;
	delete m_pGrid;
}

void SampleCube::init()
{
	glEnable(GL_DEPTH_TEST);


	if(!m_pCubeManager)
	{
		m_pCubeManager = new CubeManager();
		m_pCubeManager -> buildCity(COLUMNS, ROWS, SPACING);

		m_pOrbitCam = new OrbitCamera(m_pApp);
		m_pOrbitCam -> focusOn(m_pCubeManager -> getBoundsMin(), m_pCubeManager -> getBoundsMax());
	}

	printf("Successfully initialized Cube Sample (%d buildings)\n", m_pCubeManager -> getBuildingCount());
    // Only init if not already done
	/*
    if(m_buildings.empty())
    {

		for(int row = 0; row < ROWS; ++row){
			for(int col = 0; col < COLUMNS; ++col){
				Cube* pBuilding = new Cube();

				float width = wolf::randFloat(MIN_WIDTH, MAX_WIDTH);
				float height = wolf::randFloat(MIN_HEIGHT, MAX_HEIGHT);
				float depth = wolf::randFloat(MIN_DEPTH, MAX_DEPTH);
				
				// Center columns on both x and z
				pBuilding ->setPosition(glm::vec3(
					(col - (COLUMNS - 1) / 2.0f) * SPACING,
    				(height / 2.0f),
    				(row - (ROWS - 1) / 2.0f) * SPACING
				));

				pBuilding ->setScale(glm::vec3(
					width,
					height,
					depth
				));

				pBuilding ->setColor(
								wolf::randFloat(0.3f, 1.0f), 	// red
                                wolf::randFloat(0.3f, 1.0f),	// green
                                wolf::randFloat(0.3f, 1.0f) 	// blue
				);

				m_buildings.push_back(pBuilding);
			}
		}
		

		// Floor grid: 10 lines per half-space, spaced to match the cubes
		m_pGrid = new Grid3D(100, 5.0f);

		// Orbit camera: frame a box around the whole block of cubes
		m_pOrbitCam = new OrbitCamera(m_pApp);
		m_pOrbitCam->focusOn(glm::vec3(-2.0f, 0.0f, -14.0f), glm::vec3(2.0f, 2.0f, 1.0f));
	}
	printf("Successfully initialized Cube Sample\n");
	*/
}

void SampleCube::update(float dt)
{
	m_pOrbitCam -> update(dt);
	m_pCubeManager -> update(dt);
}

void SampleCube::render(int width, int height)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 mProj = m_pOrbitCam->getProjMatrix(width, height);
	glm::mat4 mView = m_pOrbitCam->getViewMatrix();

	m_pCubeManager -> render(mView, mProj);
}

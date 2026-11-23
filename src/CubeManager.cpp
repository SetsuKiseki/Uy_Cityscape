#include "CubeManager.h"

static const float MIN_WIDTH  = 0.8f;
static const float MAX_WIDTH  = 6.0f;
static const float MIN_HEIGHT = 0.8f;
static const float MAX_HEIGHT = 6.0f;
static const float MIN_DEPTH  = 0.8f;
static const float MAX_DEPTH  = 6.0f;

CubeManager::CubeManager()
{
}

CubeManager::~CubeManager()
{
    clear();
}

void CubeManager::clear()
{
    for(Cube* pBuilding : m_buildings)
        delete pBuilding;
    m_buildings.clear();

    delete m_pGround;
    m_pGround = nullptr;
}

void CubeManager::buildCity(int columns, int rows, float spacing)
{
    clear();

    const float groundWidth = columns * spacing + 2.0f * spacing;
    const float groundDepth = rows * spacing + 2.0f * spacing;

    m_pGround = new Plane(0);
    m_pGround -> setScale(glm::vec3(
        groundWidth,
        1.0f,
        groundDepth)
    );
    m_pGround -> setColor(0.25f, 0.25f, 0.28f);

    // prevents buildings frmo touching neighbours
    const float maxWidth = wolf::min(MAX_WIDTH, spacing * 0.9f);
    const float maxDepth = wolf::min(MAX_DEPTH, spacing * 0.9f);

    m_buildings.reserve(columns* rows);

    for(int row = 0; row < rows; ++row)
    {
        for(int col = 0; col < columns; ++col)
        {
            Cube* pBuilding = new Cube();

            float width = wolf::randFloat(MIN_WIDTH, maxWidth);
            float height = wolf::randFloat(MIN_HEIGHT, MAX_HEIGHT);
            float depth = wolf::randFloat(MIN_DEPTH, maxDepth);

            pBuilding -> setScale(glm::vec3(width, height, depth));

            // grid position centered on the origin
            float x = (col - (columns - 1) / 2.0f) * spacing;
            float z = (row - (rows - 1) / 2.0f) * spacing;

            pBuilding -> setPosition(glm::vec3(x, height / 2.0f, z));

            pBuilding -> setColor(
                wolf::randFloat(0.3f, 1.0f),
                wolf::randFloat(0.3f, 1.0f),
                wolf::randFloat(0.3f, 1.0f)
            );

            m_buildings.push_back(pBuilding);
        }
    }

    m_boundsMin = glm::vec3(-groundWidth / 2.0f, 0.0f, -groundDepth / 2.0f);
    m_boundsMax = glm::vec3(groundWidth / 2.0f, MAX_HEIGHT, groundDepth / 2.0f);
}

void CubeManager::update(float dt)
{
    //placeholder
}

void CubeManager::render(const glm::mat4& view, const glm::mat4& proj)
{
    if(m_pGround)
        m_pGround -> render(view, proj);
    
    for (Cube* pBuilding : m_buildings)
        pBuilding -> render(view, proj);
}
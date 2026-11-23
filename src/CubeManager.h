#pragma once
#include <vector>
#include "../wolf/wolf.h"
#include "Cube.h"
#include "Plane.h"

class CubeManager
{
public:
    CubeManager();
    ~CubeManager();

    CubeManager(const CubeManager&) = delete;
    CubeManager& operator = (const CubeManager&) = delete;

    void buildCity(int columns, int rows, float spacing); // builds the ground and buildings
    void clear();
    void update(float dt);
    void render(const glm::mat4& view, const glm::mat4& proj);

    int getBuildingCount() const {return (int)m_buildings.size();}

    const glm::vec3& getBoundsMin() const {return m_boundsMin;}
    const glm::vec3& getBoundsMax() const {return m_boundsMax;}

private:
    std::vector<Cube*> m_buildings;
    Plane* m_pGround = nullptr;

    glm::vec3 m_boundsMin = glm::vec3(0.0f);
    glm::vec3 m_boundsMax = glm::vec3(0.0f);
};
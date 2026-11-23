#pragma once
#include <vector>
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "Cube.h"
#include "../samplefw/OrbitCamera.h"
#include "../samplefw/Grid3D.h"

class SampleCube: public Sample
{
public:
    SampleCube(wolf::App* pApp) : Sample(pApp,"Cube") {}
    ~SampleCube();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    static const int COLUMNS = 3;
    static const int ROWS = 3;

    std::vector<Cube*> m_buildings;

    Cube*        m_pGround   = nullptr;
    OrbitCamera* m_pOrbitCam = nullptr;
    Grid3D*      m_pGrid     = nullptr;
};

#pragma once
#include <vector>
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "CubeManager.h"
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
    static const int COLUMNS = 20;
    static const int ROWS = 20;

    CubeManager* m_pCubeManager   = nullptr;
    OrbitCamera* m_pOrbitCam = nullptr;
    Grid3D*      m_pGrid     = nullptr;
};

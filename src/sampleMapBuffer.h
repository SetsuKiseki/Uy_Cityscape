#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "../samplefw/OrbitCamera.h"

class SampleMapBuffer: public Sample
{
public:
    SampleMapBuffer(wolf::App* pApp) : Sample(pApp,"Demonstrating updating a VBO via glMapBuffer") {}
    ~SampleMapBuffer();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:

    GLuint m_program = 0;
    GLuint m_vbo = 0;
    GLuint m_vao = 0;

    OrbitCamera* m_pOrbitCam = nullptr;
    float m_rot = 0;
    float m_timer = 0;
};


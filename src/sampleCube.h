#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

class SampleCube: public Sample
{
public:
    SampleCube(wolf::App* pApp) : Sample(pApp,"Cube") {}
    ~SampleCube();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    GLuint m_program = 0;
    GLuint m_vbo = 0;
    GLuint m_vao = 0;
    float m_rot = 0;
};

#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

class SampleInterleaved: public Sample
{
public:
    SampleInterleaved(wolf::App* pApp) : Sample(pApp,"Interleaved Data") {}
    ~SampleInterleaved();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    GLuint m_program = 0;
    GLuint m_vboVerts = 0;
    GLuint m_vao = 0;
};
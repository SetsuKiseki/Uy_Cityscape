#pragma once
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

class SampleInterleavedIndexed: public Sample
{
public:
    SampleInterleavedIndexed(wolf::App* pApp) : Sample(pApp,"Interleaved with Index Buffers") {}
    ~SampleInterleavedIndexed();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    GLuint m_program = 0;
    GLuint m_vbo = 0;
    GLuint m_indexBuffer = 0;
    GLuint m_vao = 0;
};

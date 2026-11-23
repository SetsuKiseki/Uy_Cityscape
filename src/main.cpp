#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../wolf/wolf.h"
#include "../samplefw/SampleRunner.h"
#include "sampleInterleaved.h"
#include "sampleInterleavedIndexed.h"
#include "sampleCube.h"
#include "sampleMapBuffer.h"

class Week2: public wolf::App
{
public:
    Week2() : App("CityScape")
    {
        //m_sampleRunner.addSample(new SampleInterleaved(this));
        //m_sampleRunner.addSample(new SampleInterleavedIndexed(this));
        m_sampleRunner.addSample(new SampleCube(this));
        m_sampleRunner.addSample(new SampleMapBuffer(this));
    }

    ~Week2()
    {
    }

    void update(float dt) override
    {
        if(isKeyDown(' '))
        {
            m_lastDown = true;
        }
        else if(m_lastDown)
        {
            m_sampleRunner.nextSample();
            m_lastDown = false;
        }

        m_sampleRunner.update(dt);
    }

    void render() override
    {
        m_sampleRunner.render(m_width, m_height);
    }

private:
    SampleRunner m_sampleRunner;
    bool m_lastDown = false;
};

int main(int, char**) {
    Week2 week2;
    week2.run();
}
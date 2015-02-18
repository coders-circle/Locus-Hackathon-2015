#pragma once
#include "Sprite.h"

class Background
{
public:
    void Init(uint32_t width, uint32_t height)
    {
        m_spr.Init("sprites/backs.gif", 32, 0, 32, 32);
        w = width;
        h = height;
    }
    void Render()
    {
        for (uint32_t i=0; i<w; i+=32)
        for (uint32_t j=0; j<h; j+=32)
            m_spr.Render((float)i, (float)j);
    }

    void CleanUp()
    {
        m_spr.CleanUp();
    }

private:
    Sprite m_spr;
    uint32_t w, h;
};

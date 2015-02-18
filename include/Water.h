#pragma once
#include "Sprite.h"

extern std::default_random_engine e1;
class Water
{
public:
    void Init(uint32_t width, uint32_t height)
    {
        m_spr.Init("sprites/backs.gif", 0, 0, 4, 4);
        m_width = width;
        m_height = height;
    }

    void Render()
    {
        std::uniform_int_distribution<int> rnd(0, 2);
        for (float x=0; x<m_width; x+=4)
        {
            float d = (float)rnd(e1);
            for (float y=(float)m_height-4; y>(float)m_height-4*(14+d); y-=4)
                m_spr.Render(x, y);
        }
    }

    void CleanUp()
    {
        m_spr.CleanUp();
    }

private:
    Sprite m_spr;
    uint32_t m_width, m_height;
};

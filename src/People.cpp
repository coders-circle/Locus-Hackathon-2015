#include <common.h>
#include "Window.h"
#include "People.h"
#include "Resources.h"

inline uint32_t GetImg(int sprid, int dir)
{
    uint32_t indices[][4] = 
    {
        0, 1, 2, 3,
        0, 3, 7, 11,
    };
    return indices[sprid][dir];
}

extern std::default_random_engine e1;
 
void People::Init(Sprite* sprite, float x, float y)
{
    // Pick a random sprite
    std::uniform_int_distribution<int> rnd(0, (int)g_resources.peopleLen-1);
    m_sprId = rnd(e1);
    auto it = g_resources.peopleStart;
    for (int i=0; i<m_sprId; ++i)
        ++it;
    m_spr = &*it;
    m_moving = true;
    m_x = x;
    m_y = y;
}

void People::Update(double dt)
{
    if (!m_spr)
        return;
    if (m_moving)
    {
            
    }
}

void People::Render()
{
    if (!m_spr)
        return;
    m_spr->Render(m_x, m_y, 1.0f, 1.0f, GetImg(m_sprId, (int)m_dir));
}


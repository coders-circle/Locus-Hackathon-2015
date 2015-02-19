#include <common.h>
#include "Window.h"
#include "People.h"
#include "Resources.h"

inline uint32_t GetImg(int sprid, int dir, int imId)
{
    if (sprid == -1)
        return 0;
    uint32_t indices[][8] = 
    {
        //{ 0, 0, 1, 1, 2, 2, 3, 3, },
        { 0, 2, 6, 3, 1, 4, 5, 11, },
        { 0, 2, 6, 3, 1, 4, 5, 11, },
        { 0, 2, 6, 3, 1, 4, 5, 11, },
    };
    return indices[sprid][dir*2+imId];
}

extern std::default_random_engine e1;
 
void People::Init(Sprite* sprite, float x, float y)
{
    if (!sprite)
    {
        // Pick a random sprite
        std::uniform_int_distribution<int> rnd(0, (int)g_resources.peopleLen-1);
        m_sprId = rnd(e1);
        auto it = g_resources.peopleStart;
        for (int i=0; i<m_sprId; ++i)
            ++it;
        m_spr = &*it;
    }
    else
    {
        m_spr = sprite;
        m_sprId = -1;
    }
    m_moving = false;
    m_x = x;
    m_y = y;
    m_player = false;

    m_title = "Human";
    m_info = "Health: 50\nSkill: 20"; 
    tm = 0;
    m_imId = 0;
}

void People::Update(double dt)
{
    if (!m_spr)
        return;
    if (m_moving)
    {
        tm += dt;
        if (tm > 1.0/5)
        {
            tm=0;
            m_imId = (m_imId+1)%2;
        }
    }
}

void People::Render()
{
    if (!m_spr)
        return;
    m_spr->Render(m_x, m_y, 1.0f, 1.0f, GetImg(m_sprId, (int)m_dir, m_imId), (m_player)?sf::Color::Green : sf::Color::White);
}


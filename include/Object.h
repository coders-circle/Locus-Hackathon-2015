#pragma once
#include "Sprite.h"

class Resources;
extern Resources g_resources;
enum Direction { UP, LEFT, RIGHT, DOWN };
class World;
class Object
{
public:

    virtual void Init(Sprite* sprite = NULL, float x = 0.0f, float y = 0.0f) 
    { 
        m_spr = sprite; 
        m_x = x; m_y = y;
    }
    virtual void Update(double dt) 
    {
        if (m_spr)
            m_spr->Animate(dt);
    }
    virtual void Render() 
    { 
        if (m_spr) 
            m_spr->Render(m_x, m_y);
    }
    virtual void CleanUp() {}

    virtual float GetWidth() const
    {
        if (!m_spr)
            return 16;
        return m_spr->GetWidth();
    }

    virtual float GetHeight() const
    {
        if (!m_spr)
            return 16;
        return m_spr->GetHeight();
    }

    virtual std::string GetTitle() { return ""; }
    virtual std::string GetInfo() { return ""; }

    virtual void Interact() {}

    float GetX() { return m_x; }
    float GetY() { return m_y; }
    Direction GetDir() { return m_dir; }

    void SetX(float x) { m_x = x; }
    void SetY(float y) { m_y = y; }
    void SetPos(float x, float y) { m_x = x; m_y = y; }
    void SetDir(Direction dir) { m_dir = dir; }

    virtual int GetType() { return -1; }

protected:
    Sprite* m_spr;

    float m_x, m_y;
    Direction m_dir;
    
    std::map<std::string, int> m_attributes;
};


class TestObject : public Object
{
public:
    std::string GetTitle() { return "TestTitle"; }
    std::string GetInfo() { return "TestInfo"; }
};

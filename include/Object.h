#pragma once
#include "Sprite.h"

enum Direction { LEFT, RIGHT, UP, DOWN };
class World;
class Object
{
public:
    Object() : m_dirty(false) {}

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

    virtual float GetWidth()
    {
        return m_spr->GetWidth();
    }

    virtual float GetHeight()
    {
        return m_spr->GetHeight();
    }

    float GetX() { return m_x; }
    float GetY() { return m_y; }
    Direction GetDir() { return m_dir; }
    bool IsDirty() { return m_dirty; }

    void SetX(float x) { m_x = x; m_dirty = true; }
    void SetY(float y) { m_y = y; m_dirty = true; }
    void SetPos(float x, float y) { m_x = x; m_y = y; m_dirty = true; }
    void SetDir(Direction dir) { m_dir = dir; m_dirty = true; }

protected:
    Sprite* m_spr;
    bool m_dirty;

    float m_x, m_y;
    Direction m_dir;
};


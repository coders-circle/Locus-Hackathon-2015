#pragma once

enum Direction { LEFT, RIGHT, UP, DOWN };
class Object
{
public:
    virtual void Init(Sprite* sprite = NULL) 
    { 
        m_spr = sprite; 
    }
    virtual void Update(double dt) 
    {
        if (m_spr)
            m_spr->Animate(dt);
    }
    virtual void Render() 
    { 
        if (m_spr) 
            m_spr->Render(x, y);
    }
    virtual void CleanUp() {}

    float x, y;
    Direction dir;

protected:
    Sprite* m_spr;
};


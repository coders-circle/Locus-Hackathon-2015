#pragma once
#include "Object.h"

class People : public Object
{
public:
    void Init(Sprite* sprite = NULL, float x = 0.0f, float y = 0.0f);

    void Update(double dt);
    void Render();

    int GetType() { return 1; }
    void Interact() { std::cout << "Interacting" << std::endl; }

private:
    bool m_moving;
    int m_sprId;
};

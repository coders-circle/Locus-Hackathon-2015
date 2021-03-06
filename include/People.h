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

    void SetPlayer(bool player = true) { m_player = player; }


    bool m_moving;
private:
    int m_sprId;
    bool m_player;
    int m_imId; double tm;
};


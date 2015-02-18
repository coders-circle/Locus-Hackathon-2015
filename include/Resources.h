#pragma once
#include "People.h"

struct Resources
{
    void CleanUp()
    {
        for (auto& spr : sprites)
            spr.CleanUp();
    }
    std::list<Sprite> sprites;
    Sprite* AddSprite() { sprites.push_back(Sprite()); return &*(--sprites.end()); }

    std::list<Sprite>::iterator peopleStart;
    size_t peopleLen;

    std::list<Object> walls;
    People player;

    // Add resources here
};

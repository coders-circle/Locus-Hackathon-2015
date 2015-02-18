#pragma once

struct Resources
{
    void CleanUp()
    {
        for (auto& spr : sprites)
            spr.CleanUp();

    }
    std::vector<Sprite> sprites;

    std::list<Object> walls;
    Object player;

    // Add resources here
};

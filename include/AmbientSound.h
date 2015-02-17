#pragma once

#include <common.h>
#include "AmbientSoundUnit.h"


class AmbientSound
{
public:
    void AddStaticUnit(std::string filename, glm::vec3 position, float occurance)
    {
        m_units.push_back(new AmbientSoundUnit(filename, position, occurance));
    }
private:
    std::vector<AmbientSoundUnit*> m_units;
};

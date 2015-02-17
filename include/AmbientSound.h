#pragma once

#include <common.h>
#include "AmbientSoundUnit.h"


class AmbientSound
{
public:
    void AddStaticUnit(std::string filename, sf::Vector3f position, float occurance)
    {
        m_units.push_back(new AmbientSoundUnit(filename, position, occurance));
    }
    void SetListenerPosition(sf::Vector3f position)
    {
        m_listenerPosition = position;
        for (auto unit : m_units)
        {
            unit->ApplyListenerPosition(m_listenerPosition);
        }
    }
    void Update(double dt)
    {
        for (auto unit : m_units)
        {
            unit->Update(dt);
        }
    }
private:
    std::vector<AmbientSoundUnit*> m_units;
    sf::Vector3f m_listenerPosition;
};

#pragma once

#include <common.h>

class AmbientSoundUnit
{
public:
    AmbientSoundUnit()
    {  
        m_timePassed = 0.0;
        m_defVolume = 1.0f;
        m_occurence = 1.0f;
    }
    AmbientSoundUnit(std::string filename, sf::Vector3f position, float occurance)
    {
        this->Set(filename, position, occurance);
        m_timePassed = 0.0;
        m_defVolume = 1.0f;
    }
    void ApplyListenerPosition(sf::Vector3f position)
    {
        sf::Vector3f newPos = m_position - position;
        m_sound.setPosition(newPos);
        std::cout << "Listner Pos = " << position.x << ", " << position.y << std::endl;
    }
    void Set(std::string filename, sf::Vector3f position, float occurance)
    {
        m_buffer.loadFromFile(filename);
        m_sound.setBuffer(m_buffer);
        m_position = position;
        m_occurence = occurance;
        m_sound.setPosition(m_position);
        m_sound.setAttenuation(2.0f);
        m_sound.setMinDistance(25.0f);
        //m_sound.setRelativeToListener(true);
    }
    float RandomNumber()
    {
        int resolution = 100;
        float random = static_cast<float> (rand()%resolution);
        return static_cast<float>(random / resolution);
    }
    virtual void Update(double dt)
    {
        m_timePassed += dt;
        if (m_sound.getStatus() != sf::Sound::Playing)
        {
            if (m_timePassed > 1.0)
            {
                m_timePassed = 0.0;
                float num = this->RandomNumber();
                if (num < m_occurence)
                {
                    m_sound.play();
                }
            }
        }
    }
protected:
    float m_defVolume, m_occurence;
    sf::Vector3f m_position;
    double m_timePassed;
    sf::SoundBuffer m_buffer;
    sf::Sound m_sound;
};

//class DynamicAmbientSoundUnit : public AmbientSoundUnit
//{
//public:
//    virtual void UpdatePosition(double m_totalTime)
//    {
//
//    }
//};
//
//class StaticAmbientSoundUnit : public AmbientSoundUnit
//{
//public:
//};
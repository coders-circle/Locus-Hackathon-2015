#pragma once

extern Window g_window;

struct Animation
{
    Animation() : time(0.0), imageId(0), speed(1.0), loop(true) {}
    double time;
    uint32_t imageId;
    double speed;
    bool loop;
};
class Sprite
{
public:
    Sprite() : m_texture(NULL), m_sprite(NULL), m_anim(NULL)  {}
    
    void Init(const sf::Uint8* pixels, int width, int height)
    {
        if (m_texture || m_sprite)
            return;
        m_texture = new sf::Texture;
        m_sprite = new sf::Sprite;
        m_numCols = 1; m_numRows = 1;
        
        if (!m_texture->create(width, height))
            throw Exception("Couldn't create texture");
        m_texture->update(pixels);
        m_sprite->setTexture(*m_texture);
    }
    void Init(const std::string& filename, int numCols = 1, int numRows = 1)
    {
        if (m_texture || m_sprite)
            return;
        m_numCols = numCols;
        m_numRows = numRows;
        m_texture = new sf::Texture;
        m_sprite = new sf::Sprite;
        if (!m_texture->loadFromFile("explosion.png"))
            throw Exception("Couldn't load from file: " + filename);
        m_sprite->setTexture(*m_texture);
    }
    void Init(const std::string& filename, int x, int y, int w, int h, int numCols = 1, int numRows = 1)
    {
        if (m_texture || m_sprite)
            return;
        m_numCols = numCols;
        m_numRows = numRows;
        m_texture = new sf::Texture;
        m_sprite = new sf::Sprite;
        if (!m_texture->loadFromFile("explosion.png", sf::IntRect(x, y, w, h)))
            throw Exception("Couldn't load from file: " + filename);
        m_sprite->setTexture(*m_texture);
    }
    void MakeAnimated()
    {
        if (m_anim)
            delete m_anim;
        m_anim = new Animation();
    }

    void Animate(double dt, bool * end = NULL)
    {
        if (!m_anim)
            return;
        m_anim->time += dt;
        if (end)
            *end = false;
        if (m_anim->time >= 1.0/m_anim->speed)
        {
            m_anim->time = 0;
            ++m_anim->imageId;
            if (end && m_anim->imageId >= uint32_t(m_numCols * m_numRows))
                *end = true;
            if (m_anim->loop)
                m_anim->imageId %= m_numCols * m_numRows;
        }
    }

    Animation* GetAnimData() { return m_anim; }
    
    void Render(float posX, float posY, float scale = 1.0f, float visibility = 1.0f, int imageId = -1)
    {
        if (!m_texture || !m_sprite)
            return;
        if (imageId < 0)
        {
            if (m_anim)
                imageId = m_anim->imageId;
            else
                imageId = 0;
        }
        sf::Vector2f uv((float)(imageId%m_numCols)/(float)m_numCols, (float)(imageId/m_numCols)/(float)m_numRows);
        uint32_t w = m_texture->getSize().x, h = m_texture->getSize().y;
        m_sprite->setTextureRect(sf::IntRect(int(uv.x*(float)w), int(uv.y*(float)h), w/m_numCols, h/m_numRows));
        m_sprite->setScale(scale, scale);
        m_sprite->setPosition(posX, posY);
        g_window.m_window->draw(*m_sprite);
    }

    void CleanUp()
    {
        if (m_sprite)
            delete m_sprite;
        if (m_sprite)
            delete m_texture;
    }
    
    float GetWidth() const { return (float)m_texture->getSize().x/(float)m_numCols; }
    float GetHeight() const { return (float)m_texture->getSize().y/(float)m_numRows; }


private:
    sf::Texture *m_texture;
    sf::Sprite *m_sprite;

    int m_numCols, m_numRows;
    Animation * m_anim;
};

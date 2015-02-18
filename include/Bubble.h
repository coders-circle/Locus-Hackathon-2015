#pragma once

class Bubble
{
public:
    void Init(Sprite* spr, sf::Font *font)
    {
        m_spr = spr;
        m_font = font;
    }

    void Render(float x, float y, const std::string &text)
    {
        float px = x, py = y;
        int lns = 1;
        for (char ch : text)
            if (ch == '\n')
                lns++;
        const float sprsz = 32;
        m_spr->Render(px, py, 1.0f, 1.0f, 6);
        px += sprsz;
        const int width = 5;
        for (int i=0; i<width; ++i, px+=sprsz)
            m_spr->Render(px, py, 1.0f, 1.0f, 2);
        m_spr->Render(px, py, 1.0f, 1.0f, 3);

        for (int i=0; i<lns; ++i)
        {
            py += sprsz;
            px = x;
            m_spr->Render(px, py, 1.0f, 1.0f, 1);
            px += sprsz;
            for (int i=0; i<width; ++i, px+=sprsz)
                m_spr->Render(px, py, 1.0f, 1.0f, 5);
            m_spr->Render(px, py, 1.0f, 1.0f, 1);
        }
        py += sprsz;
        px = x;
        m_spr->Render(px, py, 1.0f, 1.0f, 4);
        px += sprsz;
        for (int i=0; i<width; ++i, px+=sprsz)
            m_spr->Render(px, py, 1.0f, 1.0f, 2);
        m_spr->Render(px, py, 1.0f, 1.0f, 0);
        
        sf::Text textd;
        textd.setFont(*m_font);
        textd.setString(text.c_str());
        textd.setCharacterSize(16);
        textd.setPosition(x + 38, y+32);
        textd.setColor(sf::Color::Black);
        g_window.m_window->draw(textd);

    }

private:
    Sprite* m_spr;
    sf::Font* m_font;
};

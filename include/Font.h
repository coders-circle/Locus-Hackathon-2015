#pragma once
#include "tinyxml2/tinyxml2.h"

extern Window g_window;
class BitmapFont
{
public:
    void Initialize(const std::string& imageFile, const std::string& infoFile)
    {
        using namespace tinyxml2;
        XMLDocument doc;
        doc.LoadFile(infoFile.c_str());

        auto* font = doc.FirstChildElement("font"); 
        auto* info = font->FirstChildElement("info");
        size = info->IntAttribute("size");
        auto* chars = font->FirstChildElement("chars");
        auto* ch = chars->FirstChild();
        while (ch)
        {
            auto* elem = ch->ToElement();
            CharInfo cinfo;
            char c;
            c = elem->IntAttribute("id");
            cinfo.x = elem->IntAttribute("x");
            cinfo.y = elem->IntAttribute("y");
            cinfo.width = elem->IntAttribute("width");
            cinfo.height = elem->IntAttribute("height");
            cinfo.xoffset = elem->IntAttribute("xoffset");
            cinfo.yoffset = elem->IntAttribute("yoffset");
            cinfo.xadvance = elem->IntAttribute("xadvance");
            m_chars[c] = cinfo;
            ch = ch->NextSibling();
        }
        texture.loadFromFile(imageFile);
        sprite.setTexture(texture);
    }

    void DrawText(float x, float y, const std::string& string)
    {
        float py = y, px = 0.0f;
        for (size_t i=0; i<string.size(); ++i)
        {
            if (string[i] == '\n')
            {
                py += size;
                px = 0;
            }
            else
            {
                CharInfo& c = m_chars[string[i]];
                sprite.setTextureRect(sf::IntRect(c.x, c.y, c.width, c.height));
                sprite.setPosition(px + c.xoffset, py + c.yoffset);
                px += c.xadvance;
                g_window.m_window->draw(sprite);
            }
        }
    }

private:
    struct CharInfo
    {
        float x, y, width, height, xoffset, yoffset, xadvance;
    };
    std::map<char, CharInfo> m_chars;
    int size;

    sf::Sprite sprite;
    sf::Texture texture;
};


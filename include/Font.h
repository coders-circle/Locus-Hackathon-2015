#pragma once
#include "tinyxml2/tinyxml2.h"

class Font
{
public:
    void Initialize(const std::string& imageFile, const std::string& infoFile)
    {
/*        using namespace tinyxml2;
        XMLDocument doc;
        doc.LoadFile(infoFile.c_str());

        auto* font = doc.FirstChildElement("font");
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
        */
        m_sprite.Initialize(imageFile, 32, 32, 16, 16);
    }

    void DrawText(float x, float y, const std::string& string, float size)
    {
        float py = y, px = 0.0f;
        for (size_t i=0; i<string.size(); ++i)
        {
            if (string[i] == '\n')
            {
                py+=32.0f;
                px=0.0f;
            }
            else
            {
                m_sprite.Render(x+px, py, size, 1.0f, string[i]);
                px += (32.0f - 7.0f) * size;
            }
        }
    }

private:
    struct CharInfo
    {
        float x, y, width, height, xoffset, yoffset, xadvance;
    };
    Sprite m_sprite;
    std::map<char, CharInfo> m_chars;
};


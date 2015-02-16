#pragma once
#include "tinyxml2/tinyxml2.h"

class Font
{
public:
    void Initialize(const std::string& imageFile, const std::string& infoFile)
    {
        using namespace tinyxml2;
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
        m_sprite.Initialize(imageFile, 32, 32);
    }

private:
    struct CharInfo
    {
        float x, y, width, height, xoffset, yoffset, xadvance;
    };
    Sprite m_sprite;
    std::map<char, CharInfo> m_chars;
};

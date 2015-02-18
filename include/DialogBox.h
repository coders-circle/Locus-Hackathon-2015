#pragma once

#include <common.h>
#include "Menu.h"


class DialogBox
{
public:
    void Set(sf::RenderWindow* window, sf::Font* font, std::vector<std::string> contents)
    {
        m_window = window;
        m_font = font;
        //m_options.Set(window, font, )
        sf::Vector2i optionsPos;


        m_title.setColor(sf::Color::Black);
        m_title.setFont(*m_font);
        m_title.setString(contents[0]);
        m_title.setCharacterSize(32);
        

        optionsPos.y = (m_window->getSize().y - 50
            - (50 + font->getLineSpacing(50)) 
            - (contents.size() - 1)*(font->getLineSpacing(30))) / 2;
        optionsPos.x = (m_window->getSize().x - m_title.getLocalBounds().width)/2;

        m_title.setPosition(optionsPos.x, optionsPos.y);

        optionsPos.y += m_font->getLineSpacing(m_title.getCharacterSize()) + 50;
        optionsPos.x += 20;

        m_options.Set(m_window, m_font, optionsPos, 10);

        
        contents.erase(contents.begin(), contents.begin() + 1);
        for (auto option : contents)
        {
            m_options.AddItem(option);
        }
    }
    void Update(double dt)
    {
        m_options.Update(dt);
    }
    void Render()
    {
        m_window->draw(m_title);
        m_options.Render();
    }

    // 0 = none
    int GetSelection()
    {
        return m_options.GetSelected();
    }
protected:
    Menu m_options;
    //std::string m_titleString;
    sf::Text m_title;
    sf::RenderWindow* m_window;
    sf::Font* m_font;
};

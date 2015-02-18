#pragma once

#include <common.h>


class MenuItem
{
public:
    MenuItem()
    {

    }
    MenuItem(sf::RenderWindow* window, sf::Font& font, std::string label, sf::Vector2i position)
    {
        this->Set(window, font, label, position);
    }

    void Set(sf::RenderWindow* window, sf::Font& font, std::string label, sf::Vector2i position)
    {
        m_label = label;
        m_position = position;
        m_window = window;
        //m_font = font;
        //m_size = m_font->CalcSize(m_label);
        m_text.setFont(font);
        m_text.setPosition(position.x, position.y);
        m_text.setString(m_label);
        m_size.x = m_text.getLocalBounds().width;
        m_size.y = m_text.getCharacterSize();
        
    }
    void Render()
    {
        switch (m_state)
        {
        case Normal:
            m_text.setColor(sf::Color::Black);
            break;
        case MouseHovered:
            m_text.setColor(sf::Color::Blue);
            break;
        case MousePressed:
            m_text.setColor(sf::Color::Green);
            break;
        default:
            break;
        }
        //m_font->DrawText(m_position.x, m_position.y, m_label);
        m_window->draw(m_text);
    }
    bool HitTest()
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
        if (mousePos.x >= m_position.x && mousePos.y > m_position.y && mousePos.x < m_position.x + m_size.x && mousePos.y < m_position.y + m_size.y)
        {

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                m_state = MousePressed;
            }
            else if (m_state == MousePressed)
            {
                m_state = MouseClicked;
            }
            else
            {
                m_state = MouseHovered;
            }
        }
        else
        {
            m_state = Normal;
        }
        return m_state == MouseClicked;
    }
private:
    sf::Text m_text;
    std::string m_label;
    int m_state;
    sf::Vector2i m_position;
    sf::Vector2i m_size;
    sf::RenderWindow* m_window;
    enum States{Normal, MouseHovered, MousePressed, MouseClicked};
};

class Menu
{
public:
    void Update(double dt)
    {
        int i = 0;
        for (auto item : m_items)
        {
            ++i;
            if (item->HitTest())
            {
                m_selectedOption = i;
                break;
            }
        }
    }
    void Render()
    {
        for (auto item : m_items)
        {
            item->Render();
        }
    }
    void Set(sf::RenderWindow* window, sf::Font* font, sf::Vector2i position, int verticalOffset = 0)
    {
        m_verticalOffset = verticalOffset;
        m_window = window;
        m_font = font;
        m_position = position;
        m_nextMenuPosition = m_position;
        m_selectedOption = 0;
    }
    void AddItem(std::string itemLabel)
    {
        m_items.push_back(new MenuItem(m_window, *m_font, itemLabel, m_nextMenuPosition));
        m_nextMenuPosition.y += m_font->getLineSpacing(30) + m_verticalOffset;
    }

    // 0 = none
    int GetSelected()
    {
        return m_selectedOption;
    }
private:
    std::vector<MenuItem*> m_items;
    sf::RenderWindow* m_window;
    sf::Font* m_font;
    sf::Vector2i m_nextMenuPosition;
    sf::Vector2i m_position;
    int m_verticalOffset;
    int m_selectedOption;
};
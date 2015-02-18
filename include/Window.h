#pragma once

class Window
{
public:
    void Create(const std::wstring &title, uint32_t width, uint32_t height)
    {
       m_window = new sf::RenderWindow(sf::VideoMode(width, height), title.c_str());
    }

    void MainLoop()
    {
        bool quit = false;
        Timer timer(60.0);
        while (m_window->isOpen())
        {
            sf::Event event;
            while (m_window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    m_window->close();
                else if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                        m_window->close();
                }
            }

            timer.Update([this](double dt){ 
                if (m_update)
                    m_update(dt); 
            });
 
            m_window->clear(sf::Color::White);
            if (m_render)
                m_render();

            m_window->display();

        }
    }

    void CleanUp()
    {
        delete m_window;
    }

    void SetUpdateCallback(std::function<void(double)> f) { m_update = f; }
    void SetRenderCallback(std::function<void()> f) { m_render = f; }

    sf::RenderWindow* m_window;
private:


    std::function<void()> m_render;
    std::function<void(double)> m_update;
};

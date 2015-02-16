#pragma once

class Window
{
public:
    void Create(const std::string &title, uint32_t width, uint32_t height)
    {
       // if (SDL_Init(SDL_INIT_VIDEO) < 0)
       //     throw Exception("Couldn't initialize SDL");

       // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
       // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

       // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
       // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

       // m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
       // if (!m_window)
       //     throw Exception("Couldn't create window");

       // m_context = SDL_GL_CreateContext(m_window);
       // SDL_GL_SetSwapInterval(1);
       sf::ContextSettings settings;
       settings.depthBits = 24;
       settings.stencilBits = 8;
       settings.antialiasingLevel = 2;
       m_window = new sf::Window(sf::VideoMode(width, height), title.c_str(), sf::Style::Close, settings);
    }

    void MainLoop()
    {
        //SDL_Event e;
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

            std::string title = "FPS: " + std::to_string(timer.GetFPS());
            //SDL_SetWindowTitle(m_window, title.c_str());
            timer.Update([this](double dt){ 
                if (m_update)
                    m_update(dt); 
            });
 
          
            if (m_render)
                m_render();

           // SDL_GL_SwapWindow(m_window);
           m_window->display();

        }
        //while (!quit)
        //{
        //    while (SDL_PollEvent(&e))
        //    {
        //        if (e.type == SDL_QUIT)
        //            quit = true;
        //        else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
        //            quit = true;
        //    }

        //    std::string title = "FPS: " + std::to_string(timer.GetFPS());
        //    SDL_SetWindowTitle(m_window, title.c_str());
        //    timer.Update([this](double dt){ 
        //        if (m_update)
        //            m_update(dt); 
        //    });
 
        //  
        //    if (m_render)
        //        m_render();
        //    SDL_GL_SwapWindow(m_window);
        //}
    }

    void CleanUp()
    {
        //SDL_GL_DeleteContext(m_context);
        //SDL_DestroyWindow(m_window);
        //SDL_Quit();
        delete m_window;
    }

    void SetUpdateCallback(std::function<void(double)> f) { m_update = f; }
    void SetRenderCallback(std::function<void()> f) { m_render = f; }

private:

    //SDL_Window* m_window;
    //SDL_GLContext m_context;
    sf::Window* m_window;

    std::function<void()> m_render;
    std::function<void(double)> m_update;
};

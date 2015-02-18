#include <common.h>
#include "Timer.h"
#include "Window.h"
#include "World.h"
#include "AmbientSound.h"
#include "AI.h"
#include "Resources.h"

#define WIDTH (int(800/16)*16)
#define HEIGHT (int(600/16)*16)


Window g_window;
World g_world;
Resources g_resources;

sf::Vector3f g_playerPos(0.0f, 0.0f, 0.0f);
AmbientSound g_envSound;

void CreateWall(float x, float y)
{
    Object obj;
    obj.Init(&g_resources.sprites[0], x, y);
    g_resources.walls.push_back(obj);
    g_world.AddObject(&(*(--g_resources.walls.end())));
}
void Create16x16Sprite(Sprite& spr, uint32_t rgba)
{
    sf::Uint8 pixels[16*16*4];
    for (int i=0; i<16*16*4; i+=4)
        (*(int*)&pixels[i]) = rgba;
    spr.Init(pixels, 16, 16);

}
void Create16x32Sprite(Sprite& spr, uint32_t rgba)
{
    sf::Uint8 pixels[16*32*4];
    for (int i=0; i<16*32*4; i+=4)
        (*(int*)&pixels[i]) = rgba;
    spr.Init(pixels, 16, 32);

}
void Initialize()
{
    g_resources.sprites.push_back(Sprite());
    g_resources.sprites.push_back(Sprite());
    Create16x16Sprite(g_resources.sprites[0], 0xFF000000);
#if PHEIGHT == 16
    Create16x16Sprite(g_resources.sprites[1], 0xFF0000FF);
#else
    Create16x32Sprite(g_resources.sprites[1], 0xFF0000FF);
#endif

    g_world.Init(WIDTH, HEIGHT);
    g_world.SetViewArea(WIDTH/2, HEIGHT/2);
    
    for (float x=0; x<WIDTH; x+=16)
        CreateWall(x, 0);
    for (float y=16; y<HEIGHT; y+=16)
        CreateWall(0, y);
    for (float x=16; x<WIDTH-16; x+=16)
        CreateWall(x, HEIGHT-16);
    for (float y=16; y<HEIGHT; y+=16)
        CreateWall(WIDTH-16, y);
    
    std::random_device rd;
    std::default_random_engine e1(rd());
    std::uniform_int_distribution<int> xr(0, WIDTH/16-1);
    std::uniform_int_distribution<int> yr(0, HEIGHT/16-1);
    for (int i=0; i<200; ++i)
    {
        float x = (float)xr(e1)*16.0f, y = (float)yr(e1)*16.0f;
        if (!g_world.HasObstacle(x, y))
            CreateWall(x, y);
    }

    bool done = false;
    do
    {
        float x = (float)xr(e1)*16.0f, y = (float)yr(e1)*16.0f;
        if (done = !g_world.HasObstacle(x, y))
        {
            g_resources.player.Init(&g_resources.sprites[1], x, y);
            g_world.AddObject(&g_resources.player);
        }
    } while (!done);

    g_envSound.AddStaticUnit("cat1.wav", sf::Vector3f(10.0f, 0.0f, 0.0f), 1.0f);
    g_envSound.SetListenerPosition(g_playerPos);
}

PathFinding pf;
void HandleMousePress(float mx, float my)
{
    if (mx < 0 || my < 0 || mx >= WIDTH || my >= HEIGHT)
        return;
    if (g_world.HasObstacle(mx, my))
    {
        std::cout << "Unreachable taget: " << mx << "  " << my << std::endl;
        return;
    }
    bool snapped = (int)g_resources.player.GetX() % 16 == 0 && (int)g_resources.player.GetY() % 16 == 0;
    if (!snapped)
        return;
    int tx = int(mx/16)*16;
    int ty = int(my/16)*16;

    pf.Start(&g_resources.player, tx, ty);
}

bool mdown = false;
bool moving = false;
void Update(double dt)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (!mdown)
        {
            auto pos = g_window.m_window->mapPixelToCoords(sf::Mouse::getPosition(*g_window.m_window));
            HandleMousePress(pos.x, pos.y);
            mdown = true;
        }
    }
    else
        mdown = false;

    pf.Update();
    g_world.Update(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        g_playerPos.x--;
        g_envSound.SetListenerPosition(g_playerPos);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        g_playerPos.x++;
        g_envSound.SetListenerPosition(g_playerPos);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        g_playerPos.y++;
        g_envSound.SetListenerPosition(g_playerPos);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        g_playerPos.y--;
        g_envSound.SetListenerPosition(g_playerPos);
    }
    g_envSound.Update(dt);
}

void Render()
{
    g_world.SetCameraCenter(g_resources.player.GetX(), g_resources.player.GetY());
    g_world.Render();
}

void CleanUp()
{
    g_resources.CleanUp();
    g_world.CleanUp();
}

int main(int argc, char* argv[])
{
    try
    {
        g_window.Create(L"सुन्दर शान्त" , WIDTH, HEIGHT);
        Initialize();
        g_window.SetUpdateCallback(Update);
        g_window.SetRenderCallback(Render);
        g_window.MainLoop();
        CleanUp();
        g_window.CleanUp();
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}

#ifdef _WIN32
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")

#pragma comment(lib, "opengl32.lib")
#endif

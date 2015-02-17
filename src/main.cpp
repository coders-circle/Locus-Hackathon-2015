#include <common.h>
#include "Timer.h"
#include "Window.h"
#include "World.h"
#include "AmbientSound.h"

#define WIDTH (int(800/16)*16)
#define HEIGHT (int(600/16)*16)


Window g_window;
Sprite g_blackSprite;
Sprite g_redSprite;
World g_world;
std::list<Object> g_walls;
Object g_player;

sf::Vector3f g_playerPos(0.0f, 0.0f, 0.0f);
AmbientSound g_envSound;

void CreateWall(float x, float y)
{
    Object obj;
    obj.Init(&g_blackSprite, x, y);
    g_walls.push_back(obj);
    g_world.AddObject(&(*(--g_walls.end())));
}
void Create16x16Sprite(Sprite& spr, uint32_t rgba)
{
    sf::Uint8 pixels[16*16*4];
    for (int i=0; i<16*16*4; i+=4)
        (*(int*)&pixels[i]) = rgba;
    spr.Init(pixels, 16, 16);

}
void Initialize()
{
    Create16x16Sprite(g_blackSprite, 0xFF000000);
    Create16x16Sprite(g_redSprite, 0xFF0000FF);

    g_world.Init(WIDTH, HEIGHT);
    
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
            g_player.Init(&g_redSprite, x, y);
            g_world.AddObject(&g_player);
        }
    } while (!done);

    g_envSound.AddStaticUnit("Cat1.wav", sf::Vector3f(10.0f, 0.0f, 0.0f), 1.0f);
    g_envSound.SetListenerPosition(g_playerPos);
}
void Update(double dt)
{   
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
    g_world.Render();
}

void CleanUp()
{
    g_blackSprite.CleanUp();
    g_redSprite.CleanUp();
    g_world.CleanUp();
}

int main(int argc, char* argv[])
{
    try
    {
        g_window.Create("Testing", WIDTH, HEIGHT);
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

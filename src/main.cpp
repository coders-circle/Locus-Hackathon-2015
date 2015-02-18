#include <common.h>
#include "Timer.h"
#include "Window.h"
#include "World.h"
#include "AmbientSound.h"
#include "Menu.h"

#include "DialogBox.h"

#define WIDTH (int(800/16)*16)
#define HEIGHT (int(600/16)*16)


Window g_window;
Sprite g_blackSprite;
Sprite g_redSprite;
World g_world;
std::list<Object> g_walls;
Object g_player;

Menu g_menu;
sf::Font g_font;

DialogBox g_testDialog;

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
            g_player.Init(&g_redSprite, x, y);
            g_world.AddObject(&g_player);
        }
    } while (!done);

    g_font.loadFromFile("sansation.ttf");
    
    g_menu.Set(g_window.m_window, &g_font, sf::Vector2i(100, 400));
    g_menu.AddItem("Start Game");
    g_menu.AddItem("Exit");

    std::vector<std::string> dialogContents;
    dialogContents.push_back("What do you want to do?");
    dialogContents.push_back("> Give some advice!");
    dialogContents.push_back("> Laugh an Evil Laugh (Hu ha ha ha...)!");
    dialogContents.push_back("> Say \"Go to Hell!\"");
    dialogContents.push_back("> Ignore!");
    g_testDialog.Set(g_window.m_window, &g_font, dialogContents);
    //g_envSound.AddStaticUnit("cricket.wav", sf::Vector3f(10.0f, 0.0f, 0.0f), 0.1f);
    //g_envSound.AddStaticUnit("crickets.wav", sf::Vector3f(-10.0f, 10.0f, 0.0f), 0.1f);
    //g_envSound.SetListenerPosition(g_playerPos);
}

int tx=-1, ty=-1;
void HandleMousePress(float mx, float my)
{
    if (mx < 0 || my < 0 || mx >= WIDTH || my >= HEIGHT)
        return;
    if (g_world.HasObstacle(mx, my))
    {
        std::cout << "Unreachable taget: " << mx << "  " << my << std::endl;
        return;
    }
    tx = int(mx/16)*16;
    ty = int(my/16)*16;

    float dx = (float)tx - g_player.GetX();
    float dy = (float)ty - g_player.GetY();
    if (fabs(dx) > fabs(dy))
    {
        if (dx < 0)
            g_player.SetDir(LEFT);
        else if (dx > 0)
            g_player.SetDir(RIGHT);
    }
    else
    {
        if (dy < 0)
            g_player.SetDir(UP);
        else if (dy > 0)
            g_player.SetDir(DOWN);
    }
}

bool mdown = false;
void Update(double dt)
{
    //g_menu.Update(dt);
    g_testDialog.Update(dt);
    return;
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
    bool snapped = (int)g_player.GetX() % 16 == 0 && (int)g_player.GetY() % 16 == 0;
    if ((tx != -1 && ty != -1) || !snapped)
    {
        if (tx == g_player.GetX() && ty == g_player.GetY() && snapped)
        {
            tx = -1;
            ty = -1;
        }
        else
        {
            int trial = 0;
            if (snapped)
            {
                float dx = 0;
                float dy = 0;
                if (g_player.GetDir() == LEFT)
                    dx = -16;
                else if (g_player.GetDir() == RIGHT)
                    dx = 16;
                else if (g_player.GetDir() == UP)
                    dy = -16;
                else if (g_player.GetDir() == DOWN)
                    dy = 16;

                float nx = (float)g_player.GetX() + dx;
                float ny = (float)g_player.GetY() + dy;
                
                if (!g_world.HasObstacle(nx, ny))
                {
                    dx = (float)tx - g_player.GetX();
                    dy = (float)ty - g_player.GetY();
                    if (fabs(dx) > fabs(dy))
                    {
                        if (dx < 0 && !g_world.HasObstacle(g_player.GetX()-16, g_player.GetY()))
                        {
                            dx = -16;
                            g_player.SetDir(LEFT);
                        }
                        else if (dx > 0 && !g_world.HasObstacle(g_player.GetX()+16, g_player.GetY()))
                        {
                            g_player.SetDir(RIGHT);
                            dx = 16;
                        }
                        dy = 0;
                    }
                    else
                    {
                        if (dy < 0 && !g_world.HasObstacle(g_player.GetX(), g_player.GetY()-16))
                        {
                            g_player.SetDir(UP);
                            dy = -16;
                        }
                        if (dy > 0 && !g_world.HasObstacle(g_player.GetX(), g_player.GetY()+16))
                        {
                            g_player.SetDir(DOWN);
                            dy = 16;
                        }
                        dx = 0;
                    }
                }
                nx = (float)g_player.GetX() + dx;
                ny = (float)g_player.GetY() + dy;
                while (g_world.HasObstacle(nx, ny) && trial < 4)
                {
                    if (dx < 0)
                    {
                        g_player.SetDir(DOWN);
                        dy = 16;
                        dx = 0;
                    }
                    else if (dx > 0)
                    {
                        g_player.SetDir(UP);
                        dy = -16;
                        dx = 0;
                    }
                    else if (dy > 0)
                    {
                        g_player.SetDir(RIGHT);
                        dx = 16;
                        dy = 0;
                    }
                    else if (dy < 0)
                    {
                        g_player.SetDir(LEFT);
                        dx = -16;
                        dy = 0;
                    }
                    nx = (float)g_player.GetX() + dx;
                    ny = (float)g_player.GetY() + dy;             
                    trial++;
                }
            }
            if (trial < 4)
            {
                if (g_player.GetDir() == RIGHT)
                {
                    //if (!g_world.HasObstacle(g_player.GetX() + 16, g_player.GetY()))
                        g_player.SetX(g_player.GetX() + 1);
                }
                else if (g_player.GetDir() == LEFT)
                {
                    //if (!g_world.HasObstacle(g_player.GetX() - 16, g_player.GetY()))
                        g_player.SetX(g_player.GetX() - 1);
                }
                else if (g_player.GetDir() == DOWN)
                {
                    //if (!g_world.HasObstacle(g_player.GetX(), g_player.GetY() + 16))
                        g_player.SetY(g_player.GetY() + 1);
                }
                else if (g_player.GetDir() == UP)
                {
                    //if (!g_world.HasObstacle(g_player.GetX(), g_player.GetY() - 16))
                        g_player.SetY(g_player.GetY() - 1);
                }
            }
        }
    }

    g_world.Update(dt);
    /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
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
    g_envSound.Update(dt);*/
}

void Render()
{
    //g_world.SetCameraCenter(g_player.GetX(), g_player.GetY());
    //g_world.Render();
    //g_menu.Render();
    g_testDialog.Render();
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

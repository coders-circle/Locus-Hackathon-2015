#include <common.h>
#include "Window.h"
#include "World.h"
#include "AmbientSound.h"
#include "People.h"
#include "AI.h"
#include "Resources.h"
#include "Bubble.h"
#include "DialogBox.h"

#define WIDTH (int(640/16)*32)
#define HEIGHT (int(480/16)*32)


Window g_window;
World g_world;
Resources g_resources;
bool g_disableInput = false;

sf::Vector3f g_playerPos(0.0f, 0.0f, 0.0f);
//AmbientSound g_envSound;

Sprite* spr1;

DialogBox g_dialog;

void CreateWall(float x, float y)
{
    Object obj;
    obj.Init(NULL, x, y);
    g_resources.walls.push_back(obj);
    Object* objp = &(*(--g_resources.walls.end()));
    g_world.AddObject(objp);
}
void Create16x16Sprite(Sprite& spr, uint32_t rgba)
{
    sf::Uint8 pixels[16*16*4];
    for (int i=0; i<16*16*4; i+=4)
        (*(int*)&pixels[i]) = rgba;
    spr.Init(pixels, 16, 16);

}
void Create32x32Sprite(Sprite& spr, uint32_t rgba)
{
    sf::Uint8 pixels[32*32*4];
    for (int i=0; i<32*32*4; i+=4)
        (*(int*)&pixels[i]) = rgba;
    spr.Init(pixels, 32, 32);

}

void CreatePeopleSprite(int x, int y, int w, int h, int ncols, int nrows)
{
    Sprite* spr;
    spr = g_resources.AddSprite();
    spr->Init("sprites/people.png", x, y, w, h, ncols, nrows);
}
void CreatePeopleSprs()
{
    CreatePeopleSprite(0, 1, 32, 32*4, 1, 4);
    g_resources.peopleStart = --g_resources.sprites.end();
    CreatePeopleSprite(32, 1, 32*3, 32*4, 3, 4);
    g_resources.peopleLen = 2;
}


std::random_device rd;
std::default_random_engine e1(rd());
std::uniform_int_distribution<int> xr(0, WIDTH/16-1);
std::uniform_int_distribution<int> yr(0, HEIGHT/16-1);

sf::Font g_font;
    
void GetFreeRandom32x32(float& x, float &y);
void GetFreeRandom(float& x, float &y)
{
    GetFreeRandom32x32(x, y);
    return;
    //while(true)
    //{
    //    x = (float)xr(e1)*16.0f; y = (float)yr(e1)*16.0f;
    //    if (!g_world.GetObstacle(x, y))
    //        return;
    //}
}

void GetFreeRandom72x72(float& x, float &y)
{
    while(true)
    {
        x = (float)xr(e1)*32.0f; y = (float)yr(e1)*32.0f;
        float flag = false;
        for (float xx=x; xx<x+72; xx+=16)
        {
            for (float yy=y; yy<y+72; yy+=16)
            if (g_world.GetObstacle(xx, yy))
            {
                flag = true;
                break;
            }
            if (flag)
                break;
        }
        if (!flag)
            return;
    }
}
void GetFreeRandom32x32(float& x, float &y)
{
    while(true)
    {
        x = (float)xr(e1)*32.0f; y = (float)yr(e1)*32.0f;
        float flag = false;
        for (float xx=x; xx<x+32 && xx < WIDTH; xx+=16)
        {
            for (float yy=y; yy<y+32 && yy < HEIGHT; yy+=16)
            if (g_world.GetObstacle(xx, yy))
            {
                flag = true;
                break;
            }
            if (flag)
                break;
        }
        if (!flag && x < WIDTH && y < HEIGHT)
            return;
    }
}
#include "Map.h"

Bubble g_bubble;
void Initialize()
{
    if (!g_font.loadFromFile("DejaVuSans.ttf"))
        throw Exception("Can't load font");

    spr1 = g_resources.AddSprite();
    Create16x16Sprite(*spr1, 0xFF000000);
    //static Sprite spr2, spr3;
    //Create32x32Sprite(spr2, 0xFF0000FF);
    //Create32x32Sprite(spr3, 0xFFFF00FF);

    CreatePeopleSprs();
    auto spr = g_resources.AddSprite();
    spr->Init("sprites/box.png", 7, 1);
    g_bubble.Init(spr, &g_font);
    g_world.Init(WIDTH, HEIGHT);
    g_world.SetViewArea(WIDTH/1.5f, HEIGHT/1.5f);

    //Create Obstacles at HEIGHT - 4*16
    static Object waterwalls[WIDTH/16];
    for (int i=0; i<WIDTH/16; i++)
    for (float j = HEIGHT-4*16; j < HEIGHT; j+=16)
    {
        waterwalls[i].Init(NULL, (float)i*16, j);
        g_world.AddObject(&waterwalls[i]);
    }

    CreateHouses();
    
    for (float x=0; x<WIDTH; x+=16)
        CreateWall(x, 0);
    for (float y=16; y<HEIGHT; y+=16)
        CreateWall(0, y);
    for (float x=16; x<WIDTH-16; x+=16)
        CreateWall(x, HEIGHT-16);
    for (float y=16; y<HEIGHT; y+=16)
        CreateWall(WIDTH-16, y);
    
    //for (int i=0; i<200; ++i)
    //{
    //    float x = (float)xr(e1)*16.0f, y = (float)yr(e1)*16.0f;
    //    if (!g_world.GetObstacle(x, y))
    //        CreateWall(x, y);
    //}

    bool done = false;
    float x, y;
    GetFreeRandom32x32(x, y);
    g_resources.player.Init(NULL, x, y);
    g_world.AddObject(&g_resources.player);
    std::cout << x << "   " << y << std::endl;
    
    static People tests[32];
    for (int i=0; i<32; ++i)
    {
        GetFreeRandom(x, y);
        tests[i].Init(NULL, x, y);
        g_world.AddObject(&tests[i]);
    }

    g_dialog.Set(g_window.m_window, &g_font, {
        "What do you want to do?",
        "> Option 1",
        "> Option 2"
        });

    /*g_envSound.AddStaticUnit("cat1.wav", sf::Vector3f(10.0f, 0.0f, 0.0f), 1.0f);
    g_envSound.SetListenerPosition(g_playerPos);*/
}

PathFinding pf;
inline float SIGN(float x)
{
    if (x < 0) return -1;
    return 1;
}
bool FindNearest(float &mx, float &my)
{
    float dx = SIGN(mx - g_resources.player.GetX()) * 32;
    float dy = SIGN(my - g_resources.player.GetY()) * 32;
    if (!g_world.GetObstacle(mx+dx, my))
        mx = mx + dx;
    else if (!g_world.GetObstacle(mx, my+dy))
        my = my + dy;
    else if (!g_world.GetObstacle(mx+dx, my+dy))
    {
        mx += dx;
        my += dy;
    }
    else
        return false;
    return true;
}

bool g_interacting = false;
bool g_dialogShown = false;
double g_interactionPeriod = 0.0;
double g_maxInteractionPeriod = 1.0;
int g_ix = 0, g_iy = 0;

Object* g_currentObject = NULL;
void HandleMousePress(float mx, float my)
{
    if (mx <= 0 || my <= 0 || mx >= WIDTH || my >= HEIGHT)
        return;

    if (g_disableInput)
        return;
    // Move object to target by clicking
    bool snapped = (int)g_resources.player.GetX() % 32 == 0 && (int)g_resources.player.GetY() % 32 == 0;
    
    Object* obj;
    if (obj = g_world.GetObstacle(mx, my))
    {
        mx = float(int(mx/32)*32);
        my = float(int (my/32)*32);
        float dx = g_resources.player.GetX() - mx;
        float dy = g_resources.player.GetY() - my;
        if (fabs(dx) <= 32  && dy == 0 || fabs(dy) <= 32 && dx == 0)
        {
            if (obj != &g_resources.player)
            {
                Direction dir1, dir2;
                if (fabs(dx) > fabs(dy))
                {
                    if (dx < 0)
                    {
                        dir1 = LEFT;
                        dir2 = RIGHT;
                    }
                    else
                    {
                        dir1 = RIGHT;
                        dir2 = LEFT;
                    }
                }
                else
                {
                    if (dy < 0)
                    {
                        dir1 = UP;
                        dir2 = DOWN;
                    }
                    else
                    {
                        dir1 = DOWN;
                        dir2 = UP;
                    }
                
                }
                obj->SetDir(dir1);
                g_resources.player.SetDir(dir2);
                obj->Interact();

                if (obj->GetType() == 1) // 1 == PEOPLE
                {
                    g_disableInput = true;
                    g_interacting = true;
                    g_ix = obj->GetX() - 20;
                    g_iy = obj->GetY() - 70;
                }
                return;
            }
        }
        if (!FindNearest(mx, my))
            return;
    }
    if (!snapped)
        return;
    int tx = int(mx/32)*32;
    int ty = int(my/32)*32;

    pf.Start(&g_resources.player, tx, ty);
}

bool mdown = false, rmdown = false;
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

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if (!rmdown)
        {
            auto pos = g_window.m_window->mapPixelToCoords(sf::Mouse::getPosition(*g_window.m_window));
            g_currentObject = g_world.GetObstacle(pos.x, pos.y);
            if (g_currentObject && g_currentObject->GetTitle() != "")
                std::cout << g_currentObject->GetTitle() << ": " << g_currentObject->GetInfo() << std::endl;
            rmdown = true;
        }
    }
    else
        rmdown = false;

    pf.Update();
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
    }*/
    //g_envSound.Update(dt);
    if (g_interacting)
    {
        g_interactionPeriod += dt;
        if (g_interactionPeriod >= g_maxInteractionPeriod)
        {
            g_interactionPeriod = 0;
            g_interacting = false;
            g_dialogShown = true;
        }
    }
    if (g_dialogShown)
    {
        g_dialog.Update(dt);
        switch (g_dialog.GetSelection())
        {
        case 1:
            g_disableInput = false;
            g_dialogShown = false;
            break;
        case 2:
            g_disableInput = false;
            g_dialogShown = false;
            break;
        }
    }
}

void Render()
{
    
    g_world.SetCameraCenter(g_resources.player.GetX(), g_resources.player.GetY());
    g_world.Render();

    float px, py;
    px = g_window.m_window->getSize().x/2 - 100;
    py = g_window.m_window->getSize().y/2 - 50;

    if (g_interacting)
    {
        g_bubble.Render(px, py, "some random interaction text");
    }
    if (g_dialogShown)
    {
        g_dialog.Render();
    }
    //g_bubble.Render(px, py, "Hello World Great");

    if (g_currentObject && g_currentObject->GetTitle() != "")
    {
        std::string info;
        info = g_currentObject->GetTitle() + "\n------------\n" + g_currentObject->GetInfo();
        g_bubble.Render(0, HEIGHT-50, info);
    }
    g_world.ResetView();
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
        g_window.Create("Rise of the Village", int(800/16)*16, int(600/16)*16);
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

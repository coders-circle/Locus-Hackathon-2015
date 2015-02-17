#include <common.h>
#include "Timer.h"
#include "Window.h"
#include "Sprite.h"
#include "Font.h"
#include "Object.h"

Window g_window;
Sprite g_blackSprite;
Object g_obj1;
Sprite g_testSprite;
void Initialize()
{
    sf::Uint8 pixels[16*16*4];
    for (int i=0; i<16*16*4; ++i)
        if ((i+1)%4 == 0)
            pixels[i] = 0xFF;
        else
            pixels[i] = 0x0;
    g_blackSprite.Initialize(pixels, 16, 16);

    g_testSprite.Initialize("explosion.png", 4, 5);
    g_testSprite.MakeAnimated();
    g_testSprite.GetAnimData()->speed = 10;

    g_obj1.Init(&g_blackSprite);
}

void Update(double dt)
{
    g_testSprite.Animate(dt);
    g_obj1.Update(dt);
    g_obj1.x += dt*10;
}

void Render()
{
    g_testSprite.Render(200, 200);
    g_obj1.Render();
}

void CleanUp()
{
    g_blackSprite.CleanUp();
    g_testSprite.CleanUp();
    g_obj1.CleanUp();
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

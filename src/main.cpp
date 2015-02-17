#include <common.h>
#include "Timer.h"
#include "Window.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Font.h"
#include "AmbientSoundUnit.h"

Window g_window;
Renderer g_renderer;

Sprite g_sprite;
Animation animation;
Font g_font;

StaticAmbientSoundUnit g_su;

void Initialize()
{
    g_renderer.Initialize(WIDTH, HEIGHT);
    g_sprite.Initialize("explosion.png", 100, 100, 4, 5);
    g_font.Initialize("font1.png", "font.xml");
}

float c = 0.0f; float inc = 1.0f;
void Update(double dt)
{
    c +=dt*0.1f*inc;
    if (c>=1.0f) inc = -1.0f;
    else if (c<=0.0f) inc = 1.0f;
    g_renderer.SetBackColor(glm::vec3(c, c+0.2f, c-0.2f));
    g_sprite.Animate(animation, dt);
    animation.speed = 25;
    g_su.Update(dt);
}

void Render()
{
    g_renderer.BeginRender();

    g_sprite.Render(animation, 100, 100);
    g_font.DrawText(200, 200, "Hello\nWorld BIBEK IS GREAT", 0.5f);
}

void CleanUp()
{
    g_renderer.CleanUp();
    g_sprite.CleanUp();
}

int main(int argc, char* argv[])
{
    try
    {
        g_window.Create("Testing", WIDTH, HEIGHT);
        
        glewExperimental = true;
        GLenum err = glewInit();
        if (err != GLEW_OK)
            throw Exception(std::string((const char*)glewGetErrorString(err)));

        Initialize();
        g_window.SetUpdateCallback(Update);
        g_window.SetRenderCallback(Render);
        g_window.MainLoop();
        g_window.CleanUp();
        CleanUp();
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

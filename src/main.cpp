#define GLEW_STATIC
#include <glew/glew.h>
#include "GL/gl.h"
#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <functional>
#include <stdint.h>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WIDTH 800
#define HEIGHT 600

class Exception : public std::runtime_error
{
public:
    Exception(const std::string& errorString)
        : std::runtime_error(errorString)
    {}
};


inline std::string ReadFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (file.fail())
    {
        std::cout << "Error opening file: " << filename << std::endl;
        return "";
    }
    std::string text;
    std::string temp;
    while (std::getline(file, temp))
        text += temp + "\n";
    return text;
}


#include "Timer.h"
#include "Window.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Font.h"

Window g_window;
Renderer g_renderer;

Sprite g_sprite;
Animation animation;
Font g_font;
void Initialize()
{
    g_renderer.Initialize(WIDTH, HEIGHT);
    g_sprite.Initialize("explosion.png", 100, 100, 4, 5);
    g_font.Initialize("font.png", "font.xml");
}

float c = 0.0f; float inc = 1.0f;
void Update(double dt)
{
    c +=dt*0.1f*inc;
    if (c>=1.0f) inc = -1.0f;
    else if (c<=0.0f) inc = 1.0f;
    g_renderer.SetBackColor(glm::vec3(c, c+0.2f, c-0.2f));
    g_sprite.Animate(animation, dt);
    animation.speed = 5;
}

void Render()
{
    g_renderer.BeginRender();

    g_sprite.Render(animation, 100, 100);
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

#pragma once
#include "Object.h"
#include "Background.h"
#include "Water.h"

extern Window g_window;
class World
{
public:
    void Init(uint32_t width, uint32_t height)
    {
        m_width = width;
        m_height = height;
        m_wm = uint32_t(m_width/16);
        m_hm = uint32_t(m_height/16);
        m_obstacles.resize(m_wm*m_hm, NULL);
        SetViewArea((float)m_width, (float)m_height);

        m_miniMap.setViewport(sf::FloatRect(0.05f, 0.65f, 0.3f, 0.3f));
        m_miniMap.setSize(sf::Vector2f((float)m_width, (float)m_height));
        m_miniMap.setCenter(sf::Vector2f((float)m_width/2, (float)m_height/2));
        
        int w = g_window.m_window->getSize().x;
        int h = g_window.m_window->getSize().y;
        m_hud.setSize(sf::Vector2f(w, h));
        m_hud.setCenter(sf::Vector2f(w/2, h/2));


        m_back.Init(width, height);
        m_water.Init(width, height);
    }
    void SetViewArea(float viewWidth, float viewHeight)
    {
        m_camera.setSize(sf::Vector2f(viewWidth, viewHeight));
    }
    void AddObject(Object* object) 
    { 
        m_objects.push_back(object);
        int x = int(object->GetX()/16), y = int(object->GetY()/16);
        int w = int(object->GetWidth()/16), h = int(object->GetHeight()/16);
        if (w+x >= m_width/16)
            w -= ((w+x) - m_width/16);
        if (h+y >= m_height/16)
            h -= ((h+y) - m_height/16);
        for (int i=x; i<x+w; ++i)
        for (int j=y; j<y+h; ++j)
            m_obstacles[j*m_wm+i] = object;
        
        m_previousPos[object] = sf::Vector2i(x, y);
    }

    void Update(double dt)
    {
        for (auto obj : m_objects)
        {
            obj->Update(dt);
            // For moved objects, update the obstacle map
            //if (obj->IsDirty())
            {
                int nx = int(obj->GetX()/16), ny = int(obj->GetY()/16);
                sf::Vector2i pxy = m_previousPos[obj];
                if (nx < 0 || ny < 0 || nx > m_width || ny > m_height)
                    continue;
                if (nx != pxy.x || ny != pxy.y)
                {
                    int w = int(obj->GetWidth()/16), h = int(obj->GetHeight()/16);
                    int x = pxy.x, y = pxy.y;
                    if (w+x >= m_width/16)
                        w -= ((w+x) - m_width/16);
                    if (h+y >= m_height/16)
                        h -= ((h+y) - m_height/16);
                    for (int i=pxy.x; i<pxy.x+w; ++i)
                    for (int j=pxy.y; j<pxy.y+h; ++j)
                        m_obstacles[j*m_wm+i] = NULL;

                    x = nx; y = ny;
                    if (w+x >= m_width/16)
                        w -= ((w+x) - m_width/16);
                    if (h+y >= m_height/16)
                        h -= ((h+y) - m_height/16);
                    for (int i=nx; i<nx+w; ++i)
                    for (int j=ny; j<ny+h; ++j)
                        m_obstacles[j*m_wm+i] = obj;
                    m_previousPos[obj] = sf::Vector2i(nx, ny);
                }
            }
        }
    }
    void ResetView()
    {
        g_window.m_window->setView(m_camera);
    }

    Object* GetObstacle(float x, float y)
    {
        int xx = int(x/16), yy = int(y/16);
        return m_obstacles[xx+yy*m_wm];
    }

    void Render()
    {
        g_window.m_window->setView(m_camera);
        m_back.Render();
        m_water.Render();
        for (auto obj : m_objects)
            obj->Render();

        g_window.m_window->setView(m_miniMap);
        sf::RectangleShape shape(sf::Vector2f(m_width-20, m_height-20));
        shape.setOutlineThickness(10);
        shape.setOutlineColor(sf::Color(0, 0, 0));
        shape.setPosition(10, 10);
        shape.setFillColor(sf::Color(0, 0, 0, 0));
        m_back.Render();
        m_water.Render();
        for (auto obj : m_objects)
            obj->Render();
        g_window.m_window->draw(shape);
        g_window.m_window->setView(m_hud);
    }

    void CleanUp()
    {
        for (auto obj : m_objects)
            obj->CleanUp();
        m_back.CleanUp();
        m_water.CleanUp();
    }

    void SetCameraCenter(float x, float y)
    {
        if (x - m_camera.getSize().x/2 < 0)
            x = m_camera.getSize().x/2;
        else if (x + m_camera.getSize().x/2 > m_width)
            x = (float)m_width - m_camera.getSize().x/2;
        if (y - m_camera.getSize().y/2 < 0)
            y = m_camera.getSize().y/2;
        else if (y + m_camera.getSize().y/2 > m_height)
            y = (float)m_height - m_camera.getSize().y/2;
        m_camera.setCenter(sf::Vector2f(x, y));
    }

    const sf::View &GetCamera() const { return m_camera; }

private:
    std::vector<Object*> m_objects;
    uint32_t m_width, m_height;
    uint32_t m_wm, m_hm;

    sf::View m_camera;
    sf::View m_miniMap;
    sf::View m_hud;

    std::vector<Object*> m_obstacles;
    std::map<Object*, sf::Vector2i> m_previousPos;

    Background m_back;
    Water m_water;
};

#pragma once
#include "Texture.h"

extern Renderer g_renderer;

struct Animation
{
    Animation() : time(0.0), imageId(0), speed(1.0), loop(true) {}
    double time;
    uint32_t imageId;
    double speed;
    bool loop;
};
class Sprite
{
public:
    Sprite() : m_loaded(false)  {}

    void Initialize(const std::string& filename, float width, float height, int numCols = 1, int numRows = 1, float offsetX = 0, float offsetY = 0)
    {
        struct Vertex
        {
            glm::vec2 pos, tex;
        };

        m_numCols = numCols;
        m_numRows = numRows;
        
        float u = 1.0f/numCols, v = 1.0f/numRows;
        std::vector<Vertex> vertices = {
            { glm::vec2(-offsetX, -offsetY), glm::vec2(0.0f, 1.0f) },
            { glm::vec2(-offsetX, height-offsetY), glm::vec2(0.0f, 1.0f-v) },
            { glm::vec2(width-offsetX, height-offsetY), glm::vec2(u, 1.0f-v) },
            { glm::vec2(width-offsetX, -offsetY), glm::vec2(v, 1.0f) },
        };

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex));
        m_loaded = true;

        m_texId = Texture::LoadTexture(filename);
    }

    void Animate(Animation &animation, double dt, bool * end = NULL)
    {
        animation.time += dt;
        if (end)
            *end = false;
        if (animation.time >= 1.0/animation.speed)
        {
            animation.time = 0;
            ++animation.imageId;
            if (end && animation.imageId >= uint32_t(m_numCols * m_numRows))
                *end = true;
            if (animation.loop)
                animation.imageId %= m_numCols * m_numRows;
        }
    }

    void Render(float posX, float posY, float scale = 1.0f, float visibility = 1.0f, uint32_t imageId = 0)
    {
        if (!m_loaded)
            return;
        glm::vec2 uv((float)(imageId%m_numCols)/(float)m_numCols, -(float)(imageId/m_numCols)/(float)m_numRows);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texId);
       
        glm::mat4 transform = g_renderer.transforms.vp * glm::translate(glm::mat4(), glm::vec3(posX, posY, 0.0f))
                                    * glm::scale(glm::mat4(), glm::vec3(scale));
        glUniformMatrix4fv(g_renderer.uniforms.transform, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform2fv(g_renderer.uniforms.uv, 1, glm::value_ptr(uv));
        glUniform1i(g_renderer.uniforms.texture_sample, 0);
        glUniform1f(g_renderer.uniforms.visibility, visibility);

        glBindVertexArray(m_vao);
        glDrawArrays(GL_QUADS, 0, 4);
    }

    void Render(Animation &animation, float posX, float posY, float scale = 1.0f, float visibility = 1.0f)
    {
        Render(posX, posY, scale, visibility, animation.imageId);
    }

    void CleanUp()
    {
        if (!m_loaded)
            return;
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
        glDeleteTextures(1, &m_texId);
        m_loaded = false;
    }

private:
    GLuint m_vbo, m_vao, m_texId;
    bool m_loaded;
    int m_numCols, m_numRows;
};

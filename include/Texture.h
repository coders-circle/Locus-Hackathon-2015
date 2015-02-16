#pragma once

class Texture
{
public:
    static GLuint LoadTexture(const std::string& filename)
    {
        sf::Image img_data;
        if (!img_data.loadFromFile(filename.c_str()))
            throw Exception("Couldn't load file: " + filename);
        img_data.flipVertically();
        
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_data.getSize().x, img_data.getSize().y,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        return tex;
    }
};

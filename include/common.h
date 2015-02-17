#pragma once

#define GLEW_STATIC
#include <glew/glew.h>
#include "GL/gl.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

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



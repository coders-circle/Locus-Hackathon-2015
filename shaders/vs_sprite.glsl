#version 130

in vec4 vPosition;
in vec2 vTexCoord;

uniform mat4 transform;
uniform vec2 uv;

out vec2 fTexCoord;

void main()
{
    gl_Position = transform * vPosition;
    fTexCoord = vTexCoord + uv;
}


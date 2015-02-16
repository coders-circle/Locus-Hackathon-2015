#version 130

in vec2 fTexCoord;
out vec4 fColor;

uniform float visibility;
uniform sampler2D texture_sample;

void main()
{
    fColor = texture2D(texture_sample, fTexCoord) * vec4(1.0f, 1.0f, 1.0f, visibility);
}

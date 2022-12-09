#version 330 core
layout (location = 0) in vec3 aPosY;

out vec3 TexCoordsY;

uniform mat4 projectionY;
uniform mat4 viewY;

void main()
{
    TexCoordsY = aPosY;
    gl_Position =  projectionY * viewY * vec4(-aPosY, 1.0);
}  
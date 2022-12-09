#version 330 core
layout (location = 0) in vec3 aPos5;
layout (location = 1) in vec3 aNormal5;
layout (location = 2) in vec2 aTexCoords5;

out vec2 TexCoords5;

uniform mat4 model5;
uniform mat4 view5;
uniform mat4 projection5;

void main()
{
    TexCoords5 = aTexCoords5;    
    gl_Position = projection5 * view5 * model5 * vec4(aPos5, 1.0);
}
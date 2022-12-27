#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transModel;

void main()
{
    TexCoords = texCoord;    
    gl_Position = projection * view * model * transModel* vec4(pos, 1.0);
}
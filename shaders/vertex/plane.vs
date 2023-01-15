#version 330 core

layout (location = 0) in vec3 posText;
layout (location = 1) in vec2 texCoord;

out vec2 CoordText;
out vec3 FragPosText;

uniform mat4 transformText;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * transformText * vec4(posText, 1.0f);
    CoordText = texCoord;
    FragPosText = vec3(model * vec4(posText, 1.0));
    
} 
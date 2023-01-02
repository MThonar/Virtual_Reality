#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 texCoord;

out VS_OUT {
    vec2 TexCoords;
} vs_out;

uniform mat4 modelExpl;
uniform mat4 viewExpl;
uniform mat4 projectionExpl;
uniform mat4 transModelExpl;

void main()
{
    vs_out.TexCoords = texCoord;    
    gl_Position = projectionExpl * viewExpl * modelExpl*transModelExpl*vec4(pos, 1.0);
}
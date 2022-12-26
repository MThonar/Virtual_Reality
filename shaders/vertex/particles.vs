#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoords;


out vec3 TexCoords;
out vec4 ParticleColor;

uniform mat4 projectionPart;
uniform mat4 viewPart;
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 60.0f;
    TexCoords = texCoords;
    ParticleColor = color;
    gl_Position = projectionPart * viewPart*vec4((position * scale) + offset, 1.0);
}
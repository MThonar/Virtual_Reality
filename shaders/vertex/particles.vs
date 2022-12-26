#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoords;


out vec3 TexCoords;
out vec4 ParticleColor;

uniform mat4 projectionPart;
uniform mat4 viewPart;
uniform vec3 offset;
uniform vec4 color;
uniform mat4 modelPart;

void main()
{
    float scale = 0.05f;
    TexCoords = texCoords;
    ParticleColor = color;
    gl_Position = projectionPart * viewPart* modelPart * vec4((position * scale) + offset, 1.0);
}
#version 330 core
in vec3 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D sprite;
uniform vec4 colorPart;

void main()
{
    // color = (texture(sprite, TexCoords) * ParticleColor);
    color = colorPart;
} 
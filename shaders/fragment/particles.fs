#version 330 core
in vec2 TexCoordPart;
in vec4 ParticleColor;
out vec4 color;

uniform float transpar;
uniform vec4 colorPart;
uniform sampler2D texturePart1;
uniform sampler2D texturePart2;

void main()
{
    color = ParticleColor*vec4(mix(texture(texturePart1, TexCoordPart), texture(texturePart2, TexCoordPart), 0.0));
    if(color.a < 0.1)
        discard;
    color = color;
} 
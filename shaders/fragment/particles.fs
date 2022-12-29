#version 330 core
in vec2 TexCoordPart;

out vec4 color;

uniform vec4 colorPart;
uniform sampler2D texturePart1;
uniform sampler2D texturePart2;

void main()
{
    color = vec4(mix(texture(texturePart1, TexCoordPart), texture(texturePart2, TexCoordPart), 0.0));
    // color = texture(texturePart1, TexCoordPart)* vec4(colorPart);
} 
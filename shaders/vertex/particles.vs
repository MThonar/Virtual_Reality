#version 330 core
layout (location = 0) in vec3 positionPart;
layout (location = 2) in vec2 texCoordPart;

out vec2 TexCoordPart;
out vec4 ParticleColor;

uniform float scalePart;
uniform mat4 projectionPart;
uniform mat4 viewPart;
uniform vec3 offsetPart;
uniform vec4 colorPart;
uniform mat4 modelPart;

void main()
{
    TexCoordPart = vec2(texCoordPart.x, texCoordPart.y);;
    ParticleColor = colorPart;
    gl_Position = projectionPart * viewPart* modelPart * vec4((positionPart * scalePart) + offsetPart, 1.0);
}
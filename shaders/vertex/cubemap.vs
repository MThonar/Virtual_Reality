#version 330 core

layout (location = 0) in vec3 PosCubeMap;

out vec3 TexCoordsCubeMap;

uniform mat4 projectionCubeMap;
uniform mat4 viewCubeMap;

void main()
{
    TexCoordsCubeMap = PosCubeMap;
    gl_Position =  projectionCubeMap * viewCubeMap * vec4(-PosCubeMap, 1.0);
}  
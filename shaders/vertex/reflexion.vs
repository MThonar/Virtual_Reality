#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 modelRefl;
uniform mat4 viewRefl;
uniform mat4 projectionRefl;
uniform mat4 transformRefl;

void main()
{
    Normal = mat3(transpose(inverse(modelRefl))) * aNormal;
    Position = vec3(modelRefl * vec4(aPos, 1.0));
    gl_Position = projection * view *vec4(Position, 1.0);
}  
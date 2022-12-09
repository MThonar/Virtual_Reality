#version 330 core
out vec4 FragColorY;

in vec3 TexCoordsY;

uniform samplerCube skyboxY;

void main()
{    
    FragColorY = texture(skyboxY, TexCoordsY);
}
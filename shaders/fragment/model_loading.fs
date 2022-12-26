#version 330 core
out vec4 FragColor5;

in vec2 TexCoords5;

uniform sampler2D texture_normal;

void main()
{    
    FragColor5 = texture(texture_normal, TexCoords5);
}
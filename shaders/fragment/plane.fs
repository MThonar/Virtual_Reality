#version 330 core

in vec2 CoordText;
in vec3 FragPosText;

out vec4 FragColorText;

uniform sampler2D texture1;

void main()
{    
    // FragColorText = vec4(1.0,)*texture(texture1, CoordText);
    FragColorText = vec4(1.0f,1.0f,1.0f,1.0f)*texture(texture1, CoordText);
}
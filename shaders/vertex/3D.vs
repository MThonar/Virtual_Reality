#version 330 core

layout (location = 0) in vec3 posText;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 norm;

out vec2 CoordText;
out vec3 Normal;
out vec3 FragPosText;

uniform mat4 transformText;
uniform mat4 model3D;
uniform mat4 view3D;
uniform mat4 projection3D;

void main()
{
    gl_Position = projection3D * view3D * model3D * transformText * vec4(posText, 1.0f);
    CoordText = vec2(texCoord.x, texCoord.y);
    FragPosText = vec3(model3D * vec4(posText, 1.0));
    Normal = norm;
    
} 
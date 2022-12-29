#version 330 core

in vec2 CoordText;
in vec3 Normal;  
in vec3 FragPosText;

out vec4 FragColorText;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;

void main()
{   
    //Diffuse Light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPosText);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Ambient Light
    vec3 ambientScene = ambientStrength * lightColor;

    //Specular Light
    vec3 viewDir = normalize(viewPos - FragPosText);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    // Global
    vec3 colorScene = ambientScene + diffuse + specular;
    FragColorText = vec4(vec4(colorScene,1.0) *(mix(texture(texture1, CoordText), texture(texture2, CoordText), 0.5)));
}
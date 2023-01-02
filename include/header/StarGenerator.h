#ifndef STAR_GENERATOR_H
#define STAR_GENERATOR_H
#include <vector>
// #include "../include/stb/stb_image.h"
#include "../include/glad/glad.h"
#include <glm/glm.hpp>
#include "../include/GLFW/glfw3.h"

#include "../include/header/Shader.h"
#include "../include/header/texture.h"
// #include "../include/header/model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class StarGenerator
{
public:
    // constructor
    StarGenerator();

    void draw(Shader explosionShader, glm::vec3 camPosition, glm::vec3 StarPosition, float timeExpl, glm::vec3 lightPos, glm::mat4 projection5, glm::mat4 view5);
private: 
    void init();

};

#endif
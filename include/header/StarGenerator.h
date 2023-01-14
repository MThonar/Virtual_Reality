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


struct Star {
    glm::mat4 StarModelMatVec;
    glm::mat4 projection5Vec;
    glm::mat4 view5Vec;
    float timeExpl ;
    Star() : StarModelMatVec(1.0f), projection5Vec(1.0),view5Vec(1.0) , timeExpl(0.0f) { }
};


class StarGenerator
{
public:
    // constructor
    StarGenerator(float timeExpl);

    void draw(Shader explosionShader, glm::vec3 camPosition, glm::vec3 StarPosition, glm::vec3 lightPos, glm::mat4 projection5, glm::mat4 view5);
    void update(Shader explosionShader, glm::vec4 Position_1, glm::vec4  Position_3, float radius_1, float radius_2);
private: 
    std::vector<Star> stars;
    void init();
    bool CheckCollision_star(glm::vec4 Position_1, glm::vec4 Position_2, float radius_1, float radius_2) ;
};

#endif
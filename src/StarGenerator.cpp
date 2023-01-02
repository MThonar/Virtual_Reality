#include "../include/header/StarGenerator.h"

StarGenerator::StarGenerator()
{
    this->init();
}

void StarGenerator::draw(Shader explosionShader, glm::vec3 camPosition ,glm::vec3 StarPosition, float timeExpl, glm::vec3 lightPos, glm::mat4 projection5, glm::mat4 view5)
    {   
        explosionShader.Activate();
        glm::mat4 StarModelMat = glm::mat4(1.0f);
        glm::mat4 trans5 = glm::mat4(1.0f);
        trans5 = glm::rotate(trans5, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        explosionShader.setVec3("viewPos", camPosition); 
        explosionShader.setVec3("light",  lightPos);
        explosionShader.setMat4("projectionExpl", projection5);
        explosionShader.setMat4("viewExpl", view5);
        explosionShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        explosionShader.setFloatReal("ambient",  0.4f);
        explosionShader.setFloatReal("specularStrength",  0.8f);
        explosionShader.setFloatReal("timeExpl", timeExpl);
        StarModelMat = glm::scale(StarModelMat, glm::vec3(0.005f, 0.005f, 0.005f));
        // StarModelMat = glm::translate(StarModelMat, StarPosition); 
        unsigned int transformLocStar = glGetUniformLocation(explosionShader.ID, "transModelExpl");
        glUniformMatrix4fv(transformLocStar, 1, GL_FALSE, glm::value_ptr(trans5));
        explosionShader.setMat4("modelExpl", StarModelMat);
           
    }

void StarGenerator::init(){
     glm::mat4 StarModelMat = glm::mat4(1.0f);
}
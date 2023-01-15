#include "../include/header/StarGenerator.h"
// #include "../include/irrKlang/include/irrKlang.h"
// using namespace irrklang;
// ISoundEngine *SoundEngine = createIrrKlangDevice();

StarGenerator::StarGenerator(float timeExpl)
{
    this->init();
}

void StarGenerator::draw(Shader explosionShader, glm::vec3 camPosition ,glm::vec3 StarPosition, glm::vec3 lightPos, glm::mat4 projection5, glm::mat4 view5)
    
    {   
        explosionShader.Activate();
        glm::mat4 trans5 = glm::mat4(1.0f);
        trans5 = glm::rotate(trans5, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        explosionShader.setVec3("viewPos", camPosition); 
        explosionShader.setVec3("light",  lightPos);
        explosionShader.setMat4("projectionExpl", projection5);
        explosionShader.setMat4("viewExpl", view5);
        explosionShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        explosionShader.setFloatReal("ambient",  0.4f);
        explosionShader.setFloatReal("specularStrength",  0.8f);
        Star &s = this->stars[0];
        glm::mat4 StarModelMat = glm::mat4(1.0f);
        s.StarModelMatVec = StarModelMat ;
        s.projection5Vec = projection5 ;
        s.view5Vec = view5 ;

        StarModelMat = glm::translate(StarModelMat, StarPosition); 
        s.StarModelMatVec = StarModelMat;
        if (s.exploded){
            StarModelMat = glm::scale(StarModelMat, glm::vec3(0.000005f, 0.000005f, 0.000005f));
        }
        else{
            StarModelMat = glm::scale(StarModelMat, glm::vec3(0.005f, 0.005f, 0.005f));
        }
        

        
        unsigned int transformLocStar = glGetUniformLocation(explosionShader.ID, "transModelExpl");
        glUniformMatrix4fv(transformLocStar, 1, GL_FALSE, glm::value_ptr(trans5));
        explosionShader.setMat4("modelExpl", StarModelMat);
           
    }

void StarGenerator::init(){
    // SoundEngine->drop();
    this->stars.push_back(Star());
}

void StarGenerator::update(Shader explosionShader, glm::vec4 Position_1, glm::vec4 Position_3, float radius_1, float radius_2)
    {   
        glm::vec3 PosStar = glm::vec3(1.0f, 1.0f, 1.0f);
        Star &s = this->stars[0];
        glm::vec4 Position_2 =  s.StarModelMatVec * glm::vec4(PosStar, 1.0);
        Position_2 = Position_2 + glm::vec4(3.0f, 0.0f, 0.0f, 1.0f);
        bool collision = CheckCollision_star(Position_1, Position_2, 1.0f, 1.0f);
        float timeExplInc = s.timeExpl;
        if (collision){
            if (timeExplInc > 6.0f){
                s.StarModelMatVec = glm::scale(s.StarModelMatVec, glm::vec3(0.000005f, 0.000005f, 0.000005f));
                s.exploded = true;
            }
            else{
                s.timeExpl = timeExplInc + 0.1f;
                // SoundEngine->play2D("audio/bleep.mp3", false);
            }
        }
        explosionShader.setFloatReal("timeExpl", s.timeExpl);
        
    }


bool StarGenerator::CheckCollision_star(glm::vec4 Position_1, glm::vec4 Position_2, float radius_1, float radius_2) // AABB - AABB collision
{
    bool collisionX = Position_1.x + radius_1 >= Position_2.x &&
        Position_2.x + radius_2 >= Position_1.x;
    bool collisionY = Position_1.y + radius_1 >= Position_2.y &&
        Position_2.y + radius_2 >= Position_1.y;
    bool collisionZ = Position_1.z + radius_1 >= Position_2.z &&
        Position_2.z + radius_2 >= Position_1.z;
    return collisionX && collisionY && collisionZ;
} 
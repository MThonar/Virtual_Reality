#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include "../include/stb/stb_image.h"
#include "../include/header/Camera.h"
#include "../include/header/model.h"
#include "../include/header/EBO.h"
#include "../include/header/VBO.h"
#include "../include/header/VAO.h"
#include "../include/header/Shader.h"
#include "../include/header/texture.h"
#include "../include/header/ParticleGenerator.h"
// #include "../include/header/Particle.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>	

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

ParticleGenerator *Particles;


unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
} 

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
   
    if( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);  
    
    Shader lightShader("/home/flo/Virtual_Reality/shaders/vertex/3D.vs", "/home/flo/Virtual_Reality/shaders/fragment/light.fs"); // you can name your shader files however you like
    Shader modelShader("../shaders/vertex/model_loading.vs", "../shaders/fragment/model_loading.fs"); // you can name your shader files however you like
    Shader cubeMapShader("../shaders/vertex/cubemap.vs", "../shaders/fragment/cubemap.fs"); // you can name your shader files however you like
    Shader particleShader("/home/flo/Virtual_Reality/shaders/vertex/particles.vs", "/home/flo/Virtual_Reality/shaders/fragment/particles.fs");

    Model backpackModel("/home/flo/Virtual_Reality/object/backpack.obj");
    // Particle oneParticle("/home/flo/Virtual_Reality/object/tynanausore.obj");

    Texture textureContainer("/home/flo/Virtual_Reality/image/container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture textureAwesomeFace("/home/flo/Virtual_Reality/image/awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture textureParticles("/home/flo/Virtual_Reality/image/awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    Particles = new ParticleGenerator(particleShader, textureParticles, 5000);

    glm::mat4 model = glm::mat4(1.0f);

    float cubeMapVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,1.0f, 1.0f,0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,1.0f, 1.0f,0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,0.0f, 0.0f,0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f, 0.0f,0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 1.0f,0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 1.0f,0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f, 0.0f,0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,-1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,1.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f, 0.0f,-1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,-1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,1.0f, 1.0f,1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f, 0.0f,1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,1.0f, 1.0f,0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f, 0.0f,0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,1.0f, 1.0f,0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f, 0.0f,0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,0.0f, 1.0f,0.0f,  1.0f,  0.0f
};


    glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    VAO LightBoxVAO, cubeMapVAO, particlesVAO;
    LightBoxVAO.Bind();
	VBO LightBoxVBO(cubeVertices, sizeof(cubeVertices));
    LightBoxVAO.LinkAttrib(LightBoxVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	LightBoxVAO.LinkAttrib(LightBoxVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    LightBoxVAO.LinkAttrib(LightBoxVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    LightBoxVAO.LinkAttrib(LightBoxVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    LightBoxVAO.Unbind();
	LightBoxVBO.Unbind();


    cubeMapVAO.Bind();
	VBO cubeMapVBO(cubeMapVertices, sizeof(cubeMapVertices));
    cubeMapVAO.LinkAttrib(cubeMapVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    cubeMapVAO.Unbind();
	cubeMapVBO.Unbind();
  

    vector<std::string> faces
    {
        "/home/flo/Virtual_Reality/cubeMap/NiagaraFalls3/posx.jpg",
        "/home/flo/Virtual_Reality/cubeMap/NiagaraFalls3/negx.jpg",
        "/home/flo/Virtual_Reality/cubeMap/NiagaraFalls3/negy.jpg",
        "/home/flo/Virtual_Reality/cubeMap/NiagaraFalls3/posy.jpg",
        "/home/flo/Virtual_Reality/cubeMap/NiagaraFalls3/posz.jpg",
        "/home/flo/Virtual_Reality/cubeMap/NiagaraFalls3/negz.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces); 

    lightShader.Activate();
    textureContainer.texUnit(lightShader, "textureContainer", 0);
    textureAwesomeFace.texUnit(lightShader, "textureAwesomeFace", 1);

    modelShader.Activate();
    glUniform1i(glGetUniformLocation(modelShader.ID, "texture_diffuse5"), 1); // set it manually

    particleShader.Activate();
    glUniform1i(glGetUniformLocation(particleShader.ID, "texture_diffuse5"), 1); // set it manually


    cubeMapShader.Activate();
    cubeMapShader.setInt("skyboxY", 0);


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        model = glm::rotate(model, 0.0f, glm::vec3(0.5f, 1.0f, 0.0f)); 
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        textureContainer.Bind();
        glActiveTexture(GL_TEXTURE1);
        textureAwesomeFace.Bind();

        
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


        // CUBEMAP
        glDepthMask(GL_FALSE);
        cubeMapShader.Activate();
        glm::mat4 projectionY = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        cubeMapShader.setMat4("projectionY", projectionY);
        glm::mat4 viewY = camera.GetViewMatrix();
        viewY = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        cubeMapShader.setMat4("viewY", viewY);
        cubeMapVAO.Bind();
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

        // LIGHT BOX
        lightShader.Activate();
        lightShader.setVec3("viewPos", camera.Position); 
        lightShader.setVec3("lightPos",  lightPos);
        glm::mat4 projectionX = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightShader.setMat4("projectionX", projectionX);
        glm::mat4 viewX = camera.GetViewMatrix();
        lightShader.setMat4("viewX", viewX);
        int modelLoc = glGetUniformLocation(lightShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        LightBoxVAO.Bind();
        glm::mat4 trans3 = glm::mat4(1.0f);
        trans3 = glm::translate(trans3, lightPos);
        trans3 = glm::rotate(trans3, 0.0f, glm::vec3(1.0f, 0.0f, 1.0f));
        lightShader.setVec3("lightColor",  100.0f, 100.0f, 100.0f);
        lightShader.setFloatReal("ambient",  100.0f);
        unsigned int transformLoc3 = glGetUniformLocation(lightShader.ID, "transform_text");
        glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(trans3));
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // SMALL BOXES
        lightShader.setFloatReal("ambient",  0.1f); //sin((float)glfwGetTime()) + 1);
        for(unsigned int i = 0; i < 2; i++)
        {   
            glm::mat4 trans3 = glm::mat4(1.0f);
            trans3 = glm::translate(trans3, glm::vec3(-0.0f, -0.0f, -0.0f));
            glm::mat4 cubeModel = glm::mat4(1.0f);
            cubeModel = glm::translate(cubeModel, cubePositions[i]);
            float angle = 20.0f * i; 
            if (i%2) {
                lightShader.setVec3("lightColor",  0.0f, 1.0f, 0.0f);
                cubeModel = glm::rotate(cubeModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                trans3 = glm::rotate(trans3, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            }
            else{
                lightShader.setVec3("lightColor",  1.0f, 0.0f, 0.0f);
                cubeModel = glm::rotate(cubeModel, 0.0f, glm::vec3(1.0f, 0.3f, 0.5f));
                trans3 = glm::rotate(trans3, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            }
            lightShader.setMat4("model", cubeModel);

            unsigned int transformLoc3 = glGetUniformLocation(lightShader.ID, "transform_text");
            glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(trans3));

            // glDrawArrays(GL_TRIANGLES, 0, 36);
           
        }

        



        // particleShader.Activate();
        // glm::mat4 projection6 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 view6 = camera.GetViewMatrix();
        // particleShader.setMat4("projection6", projection6);
        // particleShader.setMat4("view6", view6);
        // glm::mat4 particle = glm::mat4(1.0f);
        // particleShader.setVec3("lightColor5",  1.0f, 1.0f, 0.0f);
        // particleShader.setFloatReal("ambient5",  1.0f);
        // glm::mat4 trans5 = glm::mat4(1.0f);
        // trans5 = glm::translate(trans3, glm::vec3(-0.0f, -0.0f, -0.0f));
        // trans5 = glm::rotate(trans5, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));   
        // particle = glm::translate(particle, glm::vec3(4.2f, 4.0f, 4.0f)); // translate it down so it's at the center of the scene
        // particle= glm::scale(particle, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        // particleShader.setMat4("model5", particle);
        // oneParticle.drawParticle(particleShader);

        // MODEL BACKPACK
        modelShader.Activate();
        glm::mat4 projection5 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view5 = camera.GetViewMatrix();
        modelShader.setMat4("projection5", projection5);
        modelShader.setMat4("view5", view5);
        glm::mat4 modelBackPack = glm::mat4(1.0f);
        modelShader.setVec3("lightColor5",  1.0f, 1.0f, 1.0f);
        modelShader.setFloatReal("ambient5",  1.0f);
        glm::mat4 trans4 = glm::mat4(1.0f);
        trans4 = glm::translate(trans3, glm::vec3(-0.0f, -0.0f, -0.0f));
        trans4 = glm::rotate(trans4, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));   
        modelBackPack = glm::translate(modelBackPack, glm::vec3(4.2f, 4.0f, 4.0f)); // translate it down so it's at the center of the scene
        modelBackPack= glm::scale(modelBackPack, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down
        modelShader.setMat4("model5", modelBackPack);
        backpackModel.Draw(modelShader);

        // PARTICLES
        particleShader.Activate();
        glm::mat4 projectionPart = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 viewPart = camera.GetViewMatrix();
        particleShader.setMat4("projectionPart", projectionPart);
        particleShader.setMat4("viewPart", viewPart);   
        Particles->Draw(); 
        Particles->Update(deltaTime, 2000, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
 
        
        glfwSwapBuffers(window);

        glfwPollEvents();
        glfwSwapInterval(1);
    }
    LightBoxVAO.Delete();
    LightBoxVBO.Delete();
    std::cout << "k13" << std::endl;
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    glfwSetWindowShouldClose(window, false);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
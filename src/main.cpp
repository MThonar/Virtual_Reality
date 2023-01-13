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
#include "../include/header/constants.h"

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
void renderScene(Shader &shader, Model &backpackModel);
unsigned int loadTexture(const char *path);

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

ParticleGenerator *rain;
ParticleGenerator *particleBall;

unsigned int planeVAO;

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

bool CheckCollision(glm::vec4 Position_1, glm::vec4 Position_2, float radius_1, float radius_2) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = Position_1.x + radius_1 >= Position_2.x &&
        Position_2.x + radius_2 >= Position_1.x;
    // collision y-axis?
    bool collisionY = Position_1.y + radius_1 >= Position_2.y &&
        Position_2.y + radius_2 >= Position_1.y;
    // collision only if on both axes
    bool collisionZ = Position_1.z + radius_1 >= Position_2.z &&
        Position_2.z + radius_2 >= Position_1.z;
    return collisionX && collisionY && collisionZ;
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

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);  

    // build and compile shaders
    Shader lightShader("../shaders/vertex/3D.vs", "../shaders/fragment/light.fs"); 
    Shader ModelShader("../shaders/vertex/model_loading.vs", "../shaders/fragment/model_loading.fs"); 
    Shader CubemapShader("../shaders/vertex/cubemap.vs", "../shaders/fragment/cubemap.fs"); 
    Shader rainShader("../shaders/vertex/particles.vs", "../shaders/fragment/particles.fs");
    Shader particleShader("../shaders/vertex/particles.vs", "../shaders/fragment/particles.fs");
    Shader shadow("../shaders/vertex/shadow_mapping.vs", "../shaders/fragment/shadow_mapping.fs");
    Shader simpleDepthShader("../shaders/vertex/shadow_mapping_depth.vs", "../shaders/fragment/shadow_mapping_depth.fs");

    // build and compile models
    Model BeachBallModel("../object/beachball/beachball.obj");
    Model BeachBallModel_2("../object/beachball/beachball.obj");
    stbi_set_flip_vertically_on_load(true);
    Model backpackModel("../object/backpack/backpack.obj");
    Model CloudModel("../object/CloudModel.obj");

    // build and compile textures
    Texture textureContainer("../image/container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture textureAwesomeFace("../image/awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture textureFire("../image/fireTexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    rain = new ParticleGenerator(rainShader, 200, glm::vec3(0.0f,0.4f,0.0f));
    particleBall = new ParticleGenerator(particleShader, 200, glm::vec3(0.0f,0.0f,0.0f));

    VAO LightBoxVAO, cubeMapVAO, particlesVAO;
    VBO LightBoxVBO(cubeVertices, sizeof(cubeVertices));

    // // Light Box
    // LightBoxVAO.Bind();
    // LightBoxVAO.LinkAttrib(LightBoxVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	// LightBoxVAO.LinkAttrib(LightBoxVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    // LightBoxVAO.LinkAttrib(LightBoxVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    // LightBoxVAO.LinkAttrib(LightBoxVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    // LightBoxVAO.Unbind();
	// LightBoxVBO.Unbind();

    // // Cube Map
    // VBO cubeMapVBO(cubeMapVertices, sizeof(cubeMapVertices));
    // cubeMapVAO.Bind();
    // cubeMapVAO.LinkAttrib(cubeMapVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    // cubeMapVAO.Unbind();
	// cubeMapVBO.Unbind();
    // vector<std::string> faces
    // {
    //     "../cubeMap/NiagaraFalls3/posx.jpg",
    //     "../cubeMap/NiagaraFalls3/negx.jpg",
    //     "../cubeMap/NiagaraFalls3/negy.jpg",
    //     "../cubeMap/NiagaraFalls3/posy.jpg",
    //     "../cubeMap/NiagaraFalls3/posz.jpg",
    //     "../cubeMap/NiagaraFalls3/negz.jpg"
    // };
    // unsigned int cubemapTexture = loadCubemap(faces); 

    // // Cube Map Shader
    // CubemapShader.Activate();
    // CubemapShader.setInt("skybox", 0);

    // // Light Shader
    // lightShader.Activate();
    // textureContainer.texUnit(lightShader, "texture1", 0);
    // textureAwesomeFace.texUnit(lightShader, "texture2", 1);

    // Model Shader
    // ModelShader.Activate();
    // glUniform1i(glGetUniformLocation(ModelShader.ID, "texture_diffuse"), 1); // set it manually
    // Particle Shader
    particleShader.Activate();
    textureFire.texUnit(particleShader, "texturePart1", 0);
    textureAwesomeFace.texUnit(particleShader, "texturePart2", 1);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };
    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    unsigned int woodTexture = loadTexture("../image/container.jpg");

    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------
    shadow.Activate();
    shadow.setInt("diffuseTexture", 0);
    shadow.setInt("shadowMap", 1);
    
    float angle = glm::radians(50.0f);
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        textureContainer.Bind();
        glActiveTexture(GL_TEXTURE1);
        textureAwesomeFace.Bind();  
        // glActiveTexture(GL_TEXTURE2);
        // textureFire.Bind();

        // // CUBEMAP
        // glDepthMask(GL_FALSE);
        // glm::mat4 projectionCubeMap = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 viewCubeMap = camera.GetViewMatrix();
        // viewCubeMap = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        // CubemapShader.Activate();
        // CubemapShader.setMat4("projectionCubeMap", projectionCubeMap);
        // CubemapShader.setMat4("viewCubeMap", viewCubeMap);
        // cubeMapVAO.Bind();
        // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDepthMask(GL_TRUE);


        // LIGHT BOX
        glm::mat4 model3D = glm::mat4(1.0f);
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        glm::mat4 projectionX = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view3D = camera.GetViewMatrix();
        glm::mat4 trans3 = glm::mat4(1.0f);
        // model3D = glm::rotate(model3D, 0.0f, glm::vec3(0.5f, 1.0f, 0.0f)); 
        // trans3 = glm::translate(trans3, lightPos);
        // trans3 = glm::rotate(trans3, 0.0f, glm::vec3(1.0f, 0.0f, 1.0f));
        // lightShader.Activate();
        // lightShader.setVec3("viewPos", camera.Position); 
        // lightShader.setVec3("lightPos",  lightPos);
        // lightShader.setMat4("projection3D", projectionX);
        // lightShader.setMat4("view3D", view3D);
        // lightShader.setVec3("lightColor",  100.0f, 100.0f, 100.0f);
        // lightShader.setFloatReal("ambientStrength",  100.0f);
        // lightShader.setFloatReal("specularStrength",  0.5f);
        // int modelLoc = glGetUniformLocation(lightShader.ID, "model3D");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3D));
        // LightBoxVAO.Bind(); 
        // unsigned int transformLoc3 = glGetUniformLocation(lightShader.ID, "transformText");
        // glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(trans3));
        // glDrawArrays(GL_TRIANGLES, 0, 36);


        // // SMALL BOXES
        // lightShader.setFloatReal("ambientStrength",  0.1f); 
        // lightShader.setFloatReal("specularStrength",  0.5f);
        // for(unsigned int i = 0; i < 2; i++)
        // {   
        //     glm::mat4 trans3 = glm::mat4(1.0f);
        //     trans3 = glm::translate(trans3, glm::vec3(-0.0f, -0.0f, -0.0f));
        //     glm::mat4 cubeModel = glm::mat4(1.0f);
        //     cubeModel = glm::translate(cubeModel, cubePositions[i]);
        //     float angle = 20.0f * i; 
        //     if (i%2) {
        //         lightShader.setVec3("lightColor",  0.0f, 1.0f, 0.0f);

        //         cubeModel = glm::rotate(cubeModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        //         trans3 = glm::rotate(trans3, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        //     }
        //     else{
        //         lightShader.setVec3("lightColor",  1.0f, 0.0f, 0.0f);

        //         cubeModel = glm::rotate(cubeModel, 0.0f, glm::vec3(1.0f, 0.3f, 0.5f));
        //         trans3 = glm::rotate(trans3, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        //     }
        //     lightShader.setMat4("model3D", cubeModel);

        //     unsigned int transformLoc3 = glGetUniformLocation(lightShader.ID, "transformText");
        //     glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(trans3));

        //     glDrawArrays(GL_TRIANGLES, 0, 36);
           
        // }

        // RAIN
        glActiveTexture(GL_TEXTURE0);
        textureFire.Bind();
        glActiveTexture(GL_TEXTURE1);
        textureAwesomeFace.Bind();
        rainShader.Activate();
        glm::vec3 cloudPosition = glm::vec3(3.0f, 1.0f, 3.0f);
        rain->Update(deltaTime, 300, glm::vec3(0.2f, -0.7f, 0.0f), cloudPosition);
        rain->Draw(); 
        glm::mat4 projectionRain = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 viewRain = camera.GetViewMatrix();
        glm::vec4 colRain(0.0f, 0.0f, 1.0f, 1.0f);
        glm::mat4 modelRain = glm::mat4(1.0f);
        rainShader.setMat4("projectionPart", projectionRain);
        rainShader.setMat4("viewPart", viewRain); 
        rainShader.setFloatReal("scalePart",  0.02f); 
        rainShader.setVec4("colorPart", colRain);
        rainShader.setMat4("modelPart", modelRain);
        
        // // PARTICLES BALL
        // particleShader.Activate();
        // particleBall->Update(deltaTime, 300, glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(X, Y, Z));
        
        // glm::mat4 projectionPart = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 viewPart = camera.GetViewMatrix();
        // glm::vec4 colPart(1.0f, 0.0f, 0.0f, 1.0f);
        // glm::mat4 modelPart = glm::mat4(1.0f);
        // particleShader.setMat4("projectionPart", projectionPart);
        // particleShader.setMat4("viewPart", viewPart); 
        // particleShader.setFloatReal("scalePart",  0.01f);       
        // particleShader.setVec4("colorPart", colPart);       
        // particleShader.setMat4("modelPart", modelPart);
        // particleBall->Draw(); 

        // MODELS   
        glm::mat4 projection5 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view5 = camera.GetViewMatrix();
        // glm::mat4 model = glm::mat4(1.0f);
        // glm::mat4 bb_model = glm::mat4(1.0f);
        // glm::mat4 backpack_model = glm::mat4(1.0f);
        glm::mat4 trans5 = glm::mat4(1.0f);
        ModelShader.Activate();
        // MODEL CLOUD
        // glm::vec3 cloudPosition = glm::vec3(3.0f, 1.0f, 3.0f);
        ModelShader.setVec3("viewPos", camera.Position); 
        ModelShader.setVec3("lightPos",  lightPos);
        ModelShader.setMat4("projection", projection5);
        ModelShader.setMat4("view", view5);
        ModelShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        glm::mat4 modelCloud = glm::mat4(1.0f);
        modelCloud = glm::translate(modelCloud, cloudPosition); // translate it down so it's at the center of the scene
        unsigned int transformLocCloud = glGetUniformLocation(ModelShader.ID, "transModel");
        glUniformMatrix4fv(transformLocCloud, 1, GL_FALSE, glm::value_ptr(trans5));
        ModelShader.setMat4("model", modelCloud);
        CloudModel.Draw(ModelShader);


        // BackPack MODEL
        ModelShader.setVec3("viewPos", camera.Position); 
        ModelShader.setVec3("lightPos",  lightPos);
        ModelShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        ModelShader.setMat4("projection5", projection5);
        ModelShader.setMat4("view5", view5);

        float time = (float)glfwGetTime();
        if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
            angle += 0.01f;
        }
        if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
            angle -= 0.01f;
        }
         if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            Y += 0.01f;
        }
         if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            Y -= 0.01f;
        }
         if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            X -= 0.01f;
        }
         if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            X += 0.01f;
        }

        
        trans5 = glm::rotate(trans5, (float)glfwGetTime() * glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(X, Y, Z)); // translate it down so it's at the center of the scene
        model= glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        unsigned int transformLocball = glGetUniformLocation(ModelShader.ID, "transModel");
        glUniformMatrix4fv(transformLocball, 1, GL_FALSE, glm::value_ptr(trans5));
        ModelShader.setMat4("model", model);
        BeachBallModel.Draw(ModelShader);

        bb_model = glm::translate(bb_model, glm::vec3(-2.2f, 4.0f, 0.0f)); // translate it down so it's at the center of the scene
        bb_model= glm::scale(bb_model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ModelShader.setMat4("model", bb_model);
        BeachBallModel_2.Draw(ModelShader);

        bool collision;
        glm::vec3 Pos5 = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec4 position_of_first_ball =  model * glm::vec4(Pos5, 1.0);
        glm::vec4 position_of_second_ball = bb_model * glm::vec4(Pos5, 1.0);
        collision = CheckCollision(position_of_first_ball, position_of_second_ball, 1.85f, 1.85f);
        if (collision){
            backpack_model= glm::scale(backpack_model, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
        }
        else{
            backpack_model= glm::scale(backpack_model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        }
        backpack_model = glm::translate(backpack_model, glm::vec3(-2.2f, 0.0f, 3.0f)); // translate it down so it's at the center of the scene
        ModelShader.setMat4("model", backpack_model);
        backpackModel.Draw(ModelShader);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.Activate();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        renderScene(simpleDepthShader, backpackModel);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        shadow.Activate();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shadow.setMat4("projection", projection);
        shadow.setMat4("view", view);
        // set light uniforms
        shadow.setVec3("viewPos", camera.Position);
        shadow.setVec3("lightPos", lightPos);
        shadow.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(shadow, backpackModel);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwSwapInterval(1);
    }

    LightBoxVAO.Delete();
    LightBoxVBO.Delete();

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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

// renders the 3D scene
// --------------------
void renderScene(Shader &shader, Model &backpackModel)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // backpack
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.25));
    model = glm::translate(model, glm::vec3(5.0f, 5.0f, 0.0f));
    shader.setMat4("model", model);
    backpackModel.Draw(shader);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
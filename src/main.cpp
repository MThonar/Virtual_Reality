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
#include "../include/header/StarGenerator.h"
#include "../include/header/constants.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>	


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void renderModel(Shader &shader, Model &backpackModel, Model &showerModel);
void renderSceneDepth(Shader &shader, VAO &planeVAO, Model &beachBallModel, glm::vec3 newBallPos, Model &cloudModel, glm::vec3 cloudPosition);
void renderScene(Shader &shader, Model &backpackModel, VAO &planeVAO, Model &beachBallModel, glm::vec3 newBallPos, Model &showerModel, Model &sunModel, glm::vec3 ligthPos, Model &cloudModel, glm::vec3 cloudPosition);
void renderCubemap(Shader &cubemapShader, VAO &cubemapVAO, unsigned int &cubemapTexture);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(vector<std::string> faces);

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

StarGenerator *star_one;
StarGenerator *star_two;
StarGenerator *star_three;
StarGenerator *star_four;
StarGenerator *star_five;
StarGenerator *star_six;
StarGenerator *star_7;
StarGenerator *star_8;
StarGenerator *star_9;
StarGenerator *star_10;
StarGenerator *star_11;
StarGenerator *star_12;
StarGenerator *star_13;
StarGenerator *star_15;
StarGenerator *star_14;

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        // positions
        glm::vec3 pos1(-10.0f,  -0.5f, 10.0f);
        glm::vec3 pos2(-10.0f, -0.5f, -10.0f);
        glm::vec3 pos3( 10.0f, -0.5f, -10.0f);
        glm::vec3 pos4( 10.0f,  -0.5f, 10.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 10.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(10.0f, 0.0f);  
        glm::vec2 uv4(10.0f, 10.0f);
        // normal vector
        glm::vec3 nm(0.0f, 1.0f, 0.0f);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // triangle 2
        // ----------
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        // float quadVertices[] = {
        //     // positions            // normal         // texcoords  // tangent                          // bitangent
        //     pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,nm.x, nm.y, nm.z,
        //     pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,nm.x, nm.y, nm.z,
        //     pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,nm.x, nm.y, nm.z,

        //     pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,nm.x, nm.y, nm.z,
        //     pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,nm.x, nm.y, nm.z,
        //     pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,nm.x, nm.y, nm.z,
        // };

         float quadVertices[] = {
                // positions            // normal         // texcoords  // tangent                          // bitangent                               // normal_bump
                -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  0.0f,  10.0f, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,nm.x, nm.y, nm.z,
                -10.0f, -0.5f,  -10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,nm.x, nm.y, nm.z,
                10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   10.0f, 0.0f, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,nm.x, nm.y, nm.z,
            //     pos1.x, pos1.y, pos1.z
                -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  0.0f,  0.0f, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,nm.x, nm.y, nm.z,
                10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   10.0f, 0.0f, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,nm.x, nm.y, nm.z,
                10.0f, -0.5f, 10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z, nm.x, nm.y, nm.z,
        };
         
        // configure plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(11 * sizeof(float)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(14 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


unsigned int quadVAOW = 0;
unsigned int quadVBOW;
void renderQuad_water()
{
    if (quadVAOW == 0)
    {
        // positions
        // positions
        glm::vec3 pos1(-200.0f,  -0.7f, 200.0f);
        glm::vec3 pos2(-200.0f, -0.7f, -200.0f);
        glm::vec3 pos3( 200.0f, -0.7f, -200.0f);
        glm::vec3 pos4( 200.0f,  -0.7f, 200.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 200.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(200.0f, 0.0f);  
        glm::vec2 uv4(200.0f, 200.0f);
        // normal vector
        glm::vec3 nm(0.0f, 1.0f, 0.0f);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // triangle 2
        // ----------
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
     
        float quadVertices[] = {
                // positions            // normal         // texcoords  // tangent                          // bitangent                               // normal_bump
                -200.0f, -0.7f,  200.0f,  0.0f, 1.0f, 0.0f,  0.0f,  200.0f, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,nm.x, nm.y, nm.z,
                -200.0f, -0.7f,  -200.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,nm.x, nm.y, nm.z,
                200.0f, -0.7f, -200.0f,  0.0f, 1.0f, 0.0f,   200.0f, 0.0f, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,nm.x, nm.y, nm.z,
            //     pos1.x, pos1.y, pos1.z
                -200.0f, -0.7f,  200.0f,  0.0f, 1.0f, 0.0f,  0.0f,  0.0f, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,nm.x, nm.y, nm.z,
                200.0f, -0.7f, -200.0f,  0.0f, 1.0f, 0.0f,   200.0f, 0.0f, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,nm.x, nm.y, nm.z,
                200.0f, -0.7f, 200.0f,  0.0f, 1.0f, 0.0f,  200.0f, 200.0f, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z, nm.x, nm.y, nm.z,
        };
        

         
        // configure plane VAO
        glGenVertexArrays(1, &quadVAOW);
        glGenBuffers(1, &quadVBOW);
        glBindVertexArray(quadVAOW);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBOW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(11 * sizeof(float)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(14 * sizeof(float)));
    }
    glBindVertexArray(quadVAOW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // configure global opengl states
    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    

    // build and compile shaders
    Shader lightShader("../shaders/vertex/3D.vs", "../shaders/fragment/light.fs"); 
    Shader CubemapShader("../shaders/vertex/cubemap.vs", "../shaders/fragment/cubemap.fs"); 
    Shader rainShader("../shaders/vertex/particles.vs", "../shaders/fragment/particles.fs");
    Shader particleShader("../shaders/vertex/particles.vs", "../shaders/fragment/particles.fs");
    Shader explosionShader("../shaders/vertex/explode.vs", "../shaders/fragment/explode.fs",  "../shaders/geometry/explode.gs");
    Shader shadow("../shaders/vertex/shadow_mapping.vs", "../shaders/fragment/shadow_mapping.fs");
    Shader simpleDepthShader("../shaders/vertex/shadow_mapping_depth.vs", "../shaders/fragment/shadow_mapping_depth.fs");
    Shader modelShader("../shaders/vertex/model_loading.vs", "../shaders/fragment/model_loading.fs");
    Shader shaderRender("../shaders/vertex/normal_mapping.vs", "../shaders/fragment/normal_mapping.fs");
    Shader shadow_v2("../shaders/vertex/shadow_map_v2.vs", "../shaders/fragment/shadow_map_v2.fs");

    // build and compile models
    Model beachBallModel("../object/beachball/beachball.obj");
    Model sunModel("../object/sun/sun.obj");
    stbi_set_flip_vertically_on_load(true);
    Model backpackModel("../object/backpack/backpack.obj");
    Model showerModel("../object/shower/shower.obj");
    Model starModel("../object/star/star.obj");
    Model cloudModel("../object/cloud/CloudModel.obj");
    
    // build and compile textures
    Texture textureWater("../image/dropWaterTexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture textureFire("../image/fireTexture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture textureSand("../image/grass.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    unsigned int diffuseMap = loadTexture("../image/sand.jpg");
    unsigned int normalMap  = loadTexture("../image/33991-sand-normal.png");
    unsigned int textureNormal  = loadTexture("../object/backpack/normal.png");
    unsigned int diffuseWater  = loadTexture("../image/ocean_diff.jpg");
    unsigned int normalMapWater  = loadTexture("../image/ocean.jpg");
    rain = new ParticleGenerator(rainShader, 200, glm::vec3(0.0f,0.4f,0.0f));
    particleBall = new ParticleGenerator(particleShader, 200, glm::vec3(0.0f,0.0f,0.0f));

    star_one = new StarGenerator(0.0f);
    star_two = new StarGenerator(0.0f);
    star_three = new StarGenerator(0.0f);
    star_four = new StarGenerator(0.0f);
    star_five = new StarGenerator(0.0f);
    star_six = new StarGenerator(0.0f);
    star_7 = new StarGenerator(0.0f);
    star_8 = new StarGenerator(0.0f);

    star_9 = new StarGenerator(0.0f);
    star_10 = new StarGenerator(0.0f);
    star_11 = new StarGenerator(0.0f);
    star_12 = new StarGenerator(0.0f);
    star_13 = new StarGenerator(0.0f);
    star_14 = new StarGenerator(0.0f);
    star_15 = new StarGenerator(0.0f);



    VAO LightBoxVAO;
    VBO LightBoxVBO(cubeVertices, sizeof(cubeVertices));
    // Light Box
    LightBoxVAO.Bind();
    LightBoxVAO.LinkAttrib(LightBoxVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	LightBoxVAO.LinkAttrib(LightBoxVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    LightBoxVAO.LinkAttrib(LightBoxVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    LightBoxVAO.LinkAttrib(LightBoxVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    LightBoxVAO.Unbind();
	LightBoxVBO.Unbind();


    VAO cubeMapVAO;
    VBO cubeMapVBO(cubeMapVertices, sizeof(cubeMapVertices));
    
    // Cube Map
    cubeMapVAO.Bind();
    cubeMapVAO.LinkAttrib(cubeMapVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    cubeMapVAO.Unbind();
	cubeMapVBO.Unbind();
    vector<std::string> faces
    {
        "../cubeMap/daylight/Daylight_Box_Right.jpg",
        "../cubeMap/daylight/Daylight_Box_Left.jpg",
        "../cubeMap/daylight/Daylight_Box_Bottom.jpg",
        "../cubeMap/daylight/Daylight_Box_Top.jpg",
        "../cubeMap/daylight/Daylight_Box_Front.jpg",
        "../cubeMap/daylight/Daylight_Box_Back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces); 


    // Cube Map Shader
    CubemapShader.Activate();
    CubemapShader.setInt("skybox", 0);
    
    // Particle Shader
    particleShader.Activate();
    textureFire.texUnit(particleShader, "texturePart1", 0);
    textureFire.texUnit(particleShader, "texturePart2", 1);

    shaderRender.Activate();
    shaderRender.setInt("diffuseMap", 0);
    shaderRender.setInt("normalMap", 1);

    // Plane shader
    unsigned int floorTexture = loadTexture("../image/sand.jpg");
    unsigned int grassTexture = loadTexture("../image/grass.png");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // float planeVertices[] = {
    //     // positions            // normals         // texcoords
    float planeVertices[] = {
        // positions            // normals         // texcoords
         20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
        -20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f, 20.0f,

         20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,  20.0f,  0.0f,
        -20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f, 200.0f,
         20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,  20.0f, 200.0f
    };

    VAO planeVAO;
    VBO planeVBO(planeVertices, sizeof(planeVertices));
    planeVAO.Bind();
    planeVAO.LinkAttrib(planeVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    planeVAO.LinkAttrib(planeVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    planeVAO.LinkAttrib(planeVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    planeVAO.Unbind();
    planeVBO.Unbind();

    // VAO littlePlaneVAO;
    // VBO littlePlaneVBO(littlePlaneVertices, sizeof(littlePlaneVertices));
    // littlePlaneVAO.Bind();
    // littlePlaneVAO.LinkAttrib(littlePlaneVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    // littlePlaneVAO.LinkAttrib(littlePlaneVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // littlePlaneVAO.LinkAttrib(littlePlaneVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // littlePlaneVAO.Unbind();
    // littlePlaneVBO.Unbind();

    // configure depth map FBO
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


    // // shader configuration
    shadow_v2.Activate();
    shadow_v2.setInt("diffuseTexture", 0);
    shadow_v2.setInt("shadowMap", 1);
    shadow_v2.setInt("normalMap", 2);

    // shadow.Activate();
    // shadow.setInt("diffuseTexture", 0);
    // shadow.setInt("shadowMap", 1);

    modelShader.Activate();
    modelShader.setInt("textureNormal", 0);

    shaderRender.Activate();
    shaderRender.setInt("diffuseMap", 0);
    shaderRender.setInt("normalMap", 1);

    float angle = glm::radians(00.0f);
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
        
        // Constant 
        glm::mat4 projectionX = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view3D = camera.GetViewMatrix();
        glm::vec3 lightPos(0.0f, 10.0f, -15.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        lightPos.x = cos(glfwGetTime()/5.0) * 10.0f;
        lightPos.z = cos(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime()/5.0) * 10.0f;

        float time = (float)glfwGetTime();
        glm::vec3 previous_position = glm::vec3(X, Y, Z);
        if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
            Z += 0.03f;
        }
        if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
            Z -= 0.03f;
        }
         if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            Y += 0.03f;
        }
         if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            Y -= 0.03f;
        }
         if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            X -= 0.03f;
        }
         if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            X += 0.03f;
        }
        glm::vec3 newBallPos = glm::vec3 (X,Y,Z);
        glm::vec3 cloudPosition = glm::vec3(5.0f,5.0f, 5.0f);

        // 1. render depth of scene to texture (from light's perspective)
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 40.0f;
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
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        renderSceneDepth(simpleDepthShader, planeVAO, beachBallModel, newBallPos, cloudModel, cloudPosition);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // cubeMap
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glm::mat4 projectionCubeMap = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 viewCubeMap = camera.GetViewMatrix();
        viewCubeMap = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        CubemapShader.Activate();
        CubemapShader.setMat4("projectionCubeMap", projectionCubeMap);
        CubemapShader.setMat4("viewCubeMap", viewCubeMap);
        cubeMapVAO.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        // 2. render scene as normal using the generated depth/shadow map        
        // shadow.Activate();
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 view = camera.GetViewMatrix();
        // shadow.setMat4("projection", projection);
        // shadow.setMat4("view", view);
        // // set light uniforms
        // shadow.setVec3("viewPos", camera.Position);
        // shadow.setVec3("lightPos", lightPos);
        // shadow.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, depthMap);
        // glActiveTexture(GL_TEXTURE2);
        // glBindTexture(GL_TEXTURE_2D, normalMap);
        // renderScene(shadow, backpackModel, planeVAO, beachBallModel, newBallPos, showerModel, sunModel, lightPos, cloudModel, cloudPosition);
        
        shadow_v2.Activate();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shadow_v2.setMat4("projection", projection);
        shadow_v2.setMat4("view", view);
        // set light uniforms
        shadow_v2.setVec3("viewPos", camera.Position);
        shadow_v2.setVec3("lightPos", lightPos);
        shadow_v2.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        renderScene(shadow_v2, backpackModel, planeVAO, beachBallModel, newBallPos, showerModel, sunModel, lightPos, cloudModel, cloudPosition);


        //  // Plane
        glm::mat4 projectionRender = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 viewRender = camera.GetViewMatrix();
        shaderRender.Activate();
        shaderRender.setMat4("projection", projectionRender);
        shaderRender.setMat4("view", viewRender);
        glm::mat4 modelRender = glm::mat4(1.0f);
        // modelRender = glm::rotate(modelRender, glm::radians( -90.0f), glm::normalize(glm::vec3(1.0, 0.0, 0.0))); // rotate the quad to show normal mapping from multiple directions
        // modelRender = glm::rotate(modelRender, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions
        modelRender = glm::translate(modelRender, glm::vec3(-0.0f, -0.0f, -3.0f));
        shaderRender.setMat4("model", modelRender);
        shaderRender.setVec3("viewPos", camera.Position);
        shaderRender.setVec3("lightPos", lightPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseWater);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMapWater);
        renderQuad_water();

        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 bb_model = glm::mat4(1.0f);
        bb_model = glm::translate(bb_model, glm::vec3(-2.2f, 4.0f, 0.0f)); // translate it down so it's at the center of the scene
        bb_model= glm::scale(bb_model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(X, Y, Z)); // translate it down so it's at the center of the scene
        model= glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        glm::mat4 projection5 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view5 = camera.GetViewMatrix();
        glm::vec3 Pos5 = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec4 position_of_first_ball =  model * glm::vec4(Pos5, 1.0);
        glm::vec4 position_of_second_ball = bb_model * glm::vec4(Pos5, 1.0);

        // grass
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glBindTexture(GL_TEXTURE_2D, grassTexture);
        // littlePlaneVAO.Bind();
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        // stars
        glm::vec3 StarPosition_one = glm::vec3(-3.0f, 3.0f, 3.0f);
        star_one->draw(explosionShader, camera.Position , StarPosition_one, lightPos, projection5, view5);
        star_one->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);
        
        glm::vec3 StarPosition_two = glm::vec3(3.0f, 3.0f, 4.0f);
        star_two->draw(explosionShader, camera.Position , StarPosition_two, lightPos, projection5, view5);
        star_two->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);

        glm::vec3 StarPosition_three = glm::vec3(3.0f, 3.0f, 3.0f);
        star_three->draw(explosionShader, camera.Position , StarPosition_three, lightPos, projection5, view5);
        star_three->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);

        glm::vec3 StarPosition_four = glm::vec3(-3.0f, -3.0f, 3.0f);
        star_four->draw(explosionShader, camera.Position , StarPosition_four, lightPos, projection5, view5);
        star_four->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);

        glm::vec3 StarPosition_five = glm::vec3(0.0f, 0.0f, 3.0f);
        star_five->draw(explosionShader, camera.Position , StarPosition_five, lightPos, projection5, view5);
        star_five->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);

        glm::vec3 StarPosition_six = glm::vec3(4.0f, -3.0f, 1.0f);
        star_six->draw(explosionShader, camera.Position , StarPosition_six, lightPos, projection5, view5);
        star_six->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);

        glm::vec3 StarPosition_7 = glm::vec3(5.0f, -5.0f, 6.0f);
        star_7->draw(explosionShader, camera.Position , StarPosition_7, lightPos, projection5, view5);
        star_7->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);

        glm::vec3 StarPosition_8 = glm::vec3(5.0f, 1.0f, 5.0f);
        star_8->draw(explosionShader, camera.Position , StarPosition_8, lightPos, projection5, view5);
        star_8->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);

        glm::vec3 StarPosition_9 = glm::vec3(-4.0f, 2.0f, 2.0f);
        star_9->draw(explosionShader, camera.Position , StarPosition_9, lightPos, projection5, view5);
        star_9->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);

        glm::vec3 StarPosition_10 = glm::vec3(0.0f, 3.0f, 2.0f);
        star_10->draw(explosionShader, camera.Position , StarPosition_10, lightPos, projection5, view5);
        star_10->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);

        glm::vec3 StarPosition_11 = glm::vec3(-3.0f, -5.0f, 1.0f);
        star_11->draw(explosionShader, camera.Position , StarPosition_11, lightPos, projection5, view5);
        star_11->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);


        glm::vec3 StarPosition_12 = glm::vec3(4.0f, 4.0f, 4.0f);
        star_12->draw(explosionShader, camera.Position , StarPosition_12, lightPos, projection5, view5);
        star_12->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);

        glm::vec3 StarPosition_13 = glm::vec3(-2.0f, 2.0f, 5.0f);
        star_13->draw(explosionShader, camera.Position , StarPosition_13, lightPos, projection5, view5);
        star_13->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);


        glm::vec3 StarPosition_14 = glm::vec3(-4.0f, 2.0f, 2.0f);
        star_14->draw(explosionShader, camera.Position , StarPosition_14, lightPos, projection5, view5);
        star_14->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);


        glm::vec3 StarPosition_15 = glm::vec3(-5.0f, 3.0f, 1.0f);
        star_15->draw(explosionShader, camera.Position , StarPosition_15, lightPos, projection5, view5);
        star_15->update(explosionShader, position_of_first_ball, position_of_second_ball, 1.85f, 1.85f );
        starModel.Draw(explosionShader);


        // particleBall
        glActiveTexture(GL_TEXTURE0);
        textureFire.Bind();
        glActiveTexture(GL_TEXTURE1);
        textureFire.Bind();
        particleShader.Activate();
        particleBall->Update(deltaTime, 700, glm::vec3(-3.8f,0.3f,-1.1f), glm::vec3(X, Y, Z), previous_position);
        // particleBall->Update(deltaTime, 300, glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(position_of_first_ball.x, position_of_first_ball.y, position_of_first_ball.z), angle);
        // particleBall->Update(deltaTime, 300, angle* glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(X, Y, Z));
        glm::mat4 projectionPart = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 viewPart = camera.GetViewMatrix();
        glm::vec4 colPart(1.0f, 1.0f, 1.0f, 1.0f);
        glm::mat4 modelPart = glm::mat4(1.0f);
        particleShader.setMat4("projectionPart", projectionPart);
        particleShader.setMat4("viewPart", viewPart); 
        particleShader.setFloatReal("scalePart",  0.05f);       
        particleShader.setVec4("colorPart", colPart);      
        particleShader.setMat4("modelPart", modelPart);
        particleBall->Draw(); 
    
        // RAIN
        glm::vec3 rainPosition = cloudPosition + glm::vec3(0.2, -0.4, 0.0);
        glActiveTexture(GL_TEXTURE0);
        textureWater.Bind();
        glActiveTexture(GL_TEXTURE1);
        textureWater.Bind();
        rainShader.Activate();
        rain->Update(deltaTime, 300, glm::vec3(0.0f, -0.0f, 0.0f), rainPosition, glm::vec3(0.0f, -0.0f, 0.0f));
        rain->Draw(); 
        glm::mat4 projectionRain = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 viewRain = camera.GetViewMatrix();
        glm::vec4 colRain(1.0f, 1.0f, 1.0f, 1.0f);
        glm::mat4 modelRain = glm::mat4(1.0f);
        rainShader.setMat4("projectionPart", projectionRain);
        rainShader.setMat4("viewPart", viewRain); 
        rainShader.setFloatReal("scalePart",  0.015f); 
        rainShader.setVec4("colorPart", colRain);
        rainShader.setMat4("modelPart", modelRain);

        glfwSwapBuffers(window);
        glfwPollEvents();
        // glfwSwapInterval(1);
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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
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
void renderScene(Shader &shader, Model &backpackModel, VAO &planeVAO, Model &beachBallModel, glm::vec3 newBallPos, Model &showerModel, Model &sunModel, glm::vec3 lightPos, Model &cloudModel, glm::vec3 cloudPosition)
{
    if (lightPos.y > 4.0f)
    {
        shader.setFloatReal("ambientStrength", 1.0f);
    }
    else if (lightPos.y > 2.0f && lightPos.y < 4.0f)
    {
        shader.setFloatReal("ambientStrength", (0.4 * lightPos.y) - 0.6);
    }
    else
    {
        shader.setFloatReal("ambientStrength", 0.2f);

    }
    glm::vec3 colorNeutral = glm::vec3(1.0, 1.0, 1.0);
    shader.setVec3("colorFinal", glm::vec3(1.0f, 1.0f, 0.0f));
    // floor
    // glm::mat4 model = glm::mat4(1.0f);
    // shader.setMat4("model", model);
    // planeVAO.Bind();
    // glDrawArrays(GL_TRIANGLES, 0, 6);

    glm::mat4 model = glm::mat4(1.0f);
     shader.setInt("sand", 1);
    shader.setMat4("model", model);
    shader.setMat4("model", model);
    shader.setVec3("viewPos", camera.Position);
    renderQuad();

    // BeachBall Model
    shader.setInt("sand", 0);
    shader.setVec3("colorFinal", colorNeutral);
    glm::mat4 trans5 = glm::mat4(1.0f);
    trans5 = glm::rotate(trans5, (float)glfwGetTime() * glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::translate(model, newBallPos);
    unsigned int transformLocball = glGetUniformLocation(shader.ID, "transModel");
    glUniformMatrix4fv(transformLocball, 1, GL_FALSE, glm::value_ptr(trans5));
    shader.setMat4("model", model);
    beachBallModel.Draw(shader);


    // cloud
    glm::vec3 colorCloud = glm::vec3(2.5, 2.5, 2.5);
    shader.setVec3("colorFinal", colorCloud);

    float f = 10.0;
    float j = 10.0;

    for(;f>0;f-=1.0){
        for(;j>0;j-=1.0){
    // for(unsigned int i=1;i<=10;i++){  
            model = glm::mat4(1.0f);
            model = glm::translate(model, cloudPosition+glm::vec3(f, 0.0f, j));
            shader.setMat4("model", model);
            cloudModel.Draw(shader);
        }
    }

    // sun
    shader.setVec3("colorFinal", colorNeutral);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    shader.setFloatReal("ambientStrength", 4.0f);
    shader.setMat4("model", model);
    sunModel.Draw(shader);
}

// renders the 3D scene
void renderSceneDepth(Shader &shader, VAO &planeVAO, Model &beachBallModel, glm::vec3 newBallPos, Model &cloudModel, glm::vec3 cloudPosition)
{
    // floor
    // glm::mat4 model = glm::mat4(1.0f);
    // shader.setMat4("model", model);
    // planeVAO.Bind();
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    shader.setInt("sand", 1);
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    shader.setMat4("model", model);
    shader.setVec3("viewPos", camera.Position);
    renderQuad();

    // BeachBall Model
    shader.setInt("sand", 0);
    glm::mat4 trans5 = glm::mat4(1.0f);
    trans5 = glm::rotate(trans5, (float)glfwGetTime() * glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::translate(model, newBallPos);
    unsigned int transformLocball = glGetUniformLocation(shader.ID, "transModel");
    glUniformMatrix4fv(transformLocball, 1, GL_FALSE, glm::value_ptr(trans5));
    shader.setMat4("model", model);
    beachBallModel.Draw(shader);

    float f = 20.0;
    float j = 20.0;

    for(;f>0;f-=2.0){
        for(;j>0;j-=2.0){
    // for(unsigned int i=1;i<=10;i++){  
            model = glm::mat4(1.0f);
            model = glm::translate(model, cloudPosition+glm::vec3(f, 0.0f, j));
            shader.setMat4("model", model);
            cloudModel.Draw(shader);
        }
    }
    
    
}

// Renders the 3D models
void renderModel(Shader &shader, Model &backpackModel, Model &showerModel)
{
    
    //shower
    // model = glm::translate(model, glm::vec3(2.0f, 2.0f, 0.0f));
    // showerModel.Draw(shader);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
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
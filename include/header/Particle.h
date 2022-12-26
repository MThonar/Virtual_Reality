// #ifndef PARTICULE_H
// #define PARTICULE_H

// // #include </home/leand/ULB_course/INFO-H502/playground/CPP-WORKSPACE/include/glad/glad.h> 
// // #include </home/leand/ULB_course/INFO-H502/playground/CPP-WORKSPACE/include/stb/stb_image.h>
// #include "glm/glm.hpp"
// #include "glm/gtc/matrix_transform.hpp"
// #include "glm/gtc/type_ptr.hpp"
// #include "../include/stb/stb_image.h"
// #include <iostream>
// #include <cmath>
// #include <stdio.h>
// #include <assert.h>
// #include <stdlib.h>	

// #include "assimp/Importer.hpp"
// #include "assimp/scene.h"
// #include "assimp/postprocess.h"

// #include "../include/header/mesh.h"
// #include "../include/header/Shader.h"


// #include <string>
// #include <fstream>
// #include <sstream>
// #include <iostream>
// #include <map>
// #include <vector>
// using namespace std;

// unsigned int TextureFromFileParticle(const char *path, const string &directory);

// class Particle 
// {
// public:
//     // model data 
//     vector<Texture_> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
//     vector<Mesh> meshes;
//     string directory;
//     bool gammaCorrection;

//     // constructor, expects a filepath to a 3D model.
//     Particle(string const &path);
//     // draws the model, and thus all its meshes
//     void drawParticle(Shader &shader);
//     // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
//     void loadParticle(string const &path);
//     // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
//     void processNode(aiNode *node, const aiScene *scene);
//     Mesh processMesh(aiMesh *mesh, const aiScene *scene);
//     // checks all material textures of a given type and loads the textures if they're not loaded yet.
//     // the required info is returned as a Texture struct.
//     vector<Texture_> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
// };

// unsigned int TextureFromFile(const char *path, const string &directory);
// #endif
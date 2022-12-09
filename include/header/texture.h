#ifndef TEXTURE_CLASS_H// Lets the c++ know not to open up the file twice
#define TEXTURE_CLASS_H

/// Libraries
// #include </home/flo/INFO_H502/playground/CPP-WORKSPACE/include/glad/glad.h>
// #include </home/leand/ULB_course/INFO-H502/playground/CPP-WORKSPACE/include/stb/stb_image.h>

/// Headers
#include "../include/header/Shader.h"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	// Constructor: Texture object generation 
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	// Texture unit to the texture assignment
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
#endif
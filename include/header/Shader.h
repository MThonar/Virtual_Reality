/// Shader object class ///

#ifndef SHADER_CLASS_H // Lets the c++ know not to open up the file twice
#define SHADER_CLASS_H

/// Libraries
#include "../include/glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include "glm/glm.hpp"

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor: Shader Program building from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);
	// Activation
	void Activate();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setFloatReal(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	// Delete
	void Delete();
private:
	// Shaders compilation check
	void compileErrors(unsigned int shader, const char* type);
};
#endif
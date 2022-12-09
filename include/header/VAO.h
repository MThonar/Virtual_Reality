/// Vertex array object class ///

#ifndef VAO_CLASS_H// Lets the c++ know not to open up the file twice
#define VAO_CLASS_H

/// Libraries
#include "../include/glad/glad.h"

/// Headers
#include "../include/header/VBO.h"

class VAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID;
	// Constructor: Vertex array object generation 
	VAO();
	// VBO Attribute linking such as a position or color to the VAO
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Binding
	void Bind();
	// Unbinding
	void Unbind();
	// Delete
	void Delete();
};

#endif
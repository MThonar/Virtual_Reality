/// Vertex Buffer Object class ///

#ifndef VBO_CLASS_H// Lets the c++ know not to open up the file twice
#define VBO_CLASS_H

/// Libraries
#include "../include/glad/glad.h"

// The VBO is an array of references
class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor: Vertex Buffer object creation + vertices linking
	VBO(GLfloat* vertices, GLsizeiptr size);
	// Binding
	void Bind();
	// Unbinding
	void Unbind();
	// Delete
	void Delete();
};

#endif
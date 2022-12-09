/// Element buffer object class ///

#ifndef EBO_CLASS_H// Lets the c++ know not to open up the file twice
#define EBO_CLASS_H

/// Libraries
#include "../include/glad/glad.h"

class EBO
{
public:
	// Elements Buffer Object ID reference  
	GLuint ID;
	// Constructor: Elements Buffer object generation + indices linking
	EBO(GLuint* indices, GLsizeiptr size);

	// Binding
	void Bind();
	// Unbinding
	void Unbind();
	// Delete
	void Delete();
};

#endif
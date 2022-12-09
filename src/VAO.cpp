/// Vertex array object class ///
// VAO to points to one or more VBOs and tells OpenGL how to interpret them
// Allows to quickly switch between different VBOs
//ATTENTION: Need to generate the VAO before the VBO

/// Headers
#include "../include/header/VAO.h"

// Constructor: Vertex array object generation 
VAO::VAO()
{
	// Buffer object creation (one 3D object, reference pointing)
	glGenVertexArrays(1, &ID);
}

// VBO Attribute linking such as a position or color to the VAO
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	// Binding
	VBO.Bind();
	// Configuration such that opengl knows how to read the VBO
	// (index vertex attribute, # values per vertex, types of values, if coordinate in int, # data between each vertex, pointer where the vertices begin in the array)
	// index vertex attribute allows to communicate with vertex shader from the outside
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	// Unbinding
	VBO.Unbind();
}
// Binding
void VAO::Bind()
{
	glBindVertexArray(ID);
}
// Unbinding
void VAO::Unbind()
{
	glBindVertexArray(0);
}
// Delete
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
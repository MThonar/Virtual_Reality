// VBO is an array of references

/// Headers
#include "../include/header/VBO.h"

// Constructor: Vertex Buffer object creation + vertices linking
VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{   
    // Buffer object creation (one 3D object, reference pointing)
	glGenBuffers(1, &ID);
    // Binding 
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    // Need to specify following to improve the performence
    // GL_STREAM : vertices will be modified once and used a few times
    // GL_STATIC : vertices will be modified once and used a many times
    // GL_DYNAMIC : vertices will be modified multiple times and used a many times

    // DRAW : vertices will be modified and be used to draw an image on the screen
}
// Binding
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}
// Unbinding
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// Delete
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
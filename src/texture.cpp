/// Texture object class ///

/// Headers
#include "../include/header/texture.h"
#include "../include/stb/stb_image.h"
// Constructor: Texture object generation
Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	// Assigns the type of the texture ot the texture object
	type = texType;
	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	// OpenGL texture object creation
	glGenTextures(1, &ID);
	// Texture to a Texture Unit assignment
	glActiveTexture(slot);
	glBindTexture(texType, ID);
	// Type of algorithm configuration that is used to make the image smaller or bigger
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // GL_NEAREST : keeps the pixels as they are
    // GL_LINEAR : creates new pixels according to the pixels nearby (blurring effect)
	// Texture repetition configuration (if it does at all)
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Applied on one axis (so can combine several)
    // GL_REPEAT : repeats image
    // GL_MIRRORED_REPEAT : repeats image but mirror it at each repetition
    // GL_CLAMP_TO_EDGE : stretches the border of the image
    // GL_CLAMP_TO_BORDER : put flat color of choice outside the image
	// Image to the OpenGL Texture object assignment
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	// MipMaps creation
	glGenerateMipmap(texType);
	// Delete
	stbi_image_free(bytes);
	// Unbinding
	glBindTexture(texType, 0);
}
// Texture unit to the texture assignment
void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}
// Binding
void Texture::Bind()
{
	glBindTexture(type, ID);
}
// Unbinding
void Texture::Unbind()
{
	glBindTexture(type, 0);
}
// Delete
void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
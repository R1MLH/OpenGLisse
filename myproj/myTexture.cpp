#include "myTexture.h"
#include "helperFunctions.h"
#include "myShader.h"

#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

myTexture::myTexture()
{
	texture_id = 0;
	texture_type = GL_TEXTURE_2D;
}

myTexture::myTexture(std::string filename)
{
	texture_type = GL_TEXTURE_2D;
	readTexture_2D(filename);
}

myTexture::myTexture(std::vector<std::string> & filenames)
{
	texture_type = GL_TEXTURE_CUBE_MAP;
	readTexture_cubemap(filenames);
}


myTexture::~myTexture()
{
	glDeleteTextures(1, &texture_id);
}


bool myTexture::readTexture_2D(std::string filename)
{
	int size;
	GLubyte *mytexture = stbi_load(filename.c_str(), &width, &height, &size, 4);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLuint>(width), static_cast<GLuint>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, mytexture);

	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] mytexture;
	return true;
}


void myTexture::readTexture_cubemap(std::vector<std::string>& filenames)
{
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	enum Face { LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK };

	for (Face f : {LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK})
	{
		int size, width, height;
		GLubyte *mytexture = stbi_load(filenames[f].c_str(), &width, &height, &size, 4);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + f, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mytexture);

		delete[] mytexture;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


void myTexture::bind(myShader *shader, std::string name, GLuint texture_offset )
{
	glActiveTexture(GL_TEXTURE0 + texture_offset);
	glBindTexture(texture_type, texture_id);

	shader->setUniform(name, static_cast<int>(texture_offset));
}

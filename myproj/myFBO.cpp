#include "myFBO.h"



myFBO::myFBO()
{
	colortexture = nullptr;
	depthtexture = nullptr;
	fboid = 0;
}


myFBO::~myFBO()
{
	if (colortexture != nullptr) delete colortexture;
	if (depthtexture != nullptr) delete depthtexture;
	if (fboid != 0) glDeleteFramebuffers(1, &fboid);
}

void myFBO::initFBO(int width, int height)
{
	if (colortexture) delete colortexture;
	colortexture = new myTexture();

	glGenTextures(1, &colortexture->texture_id);

	glBindTexture(GL_TEXTURE_2D, colortexture->texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);


	if (depthtexture) delete depthtexture;
	depthtexture = new myTexture();

	glGenTextures(1, &depthtexture->texture_id);

	glBindTexture(GL_TEXTURE_2D, depthtexture->texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &fboid);
	glBindFramebuffer(GL_FRAMEBUFFER, fboid);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colortexture->texture_id, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthtexture->texture_id, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void myFBO::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboid);
}

void myFBO::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void myFBO::clear()
{
	bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	unbind();
}

#pragma once
#include "glwrapper.h"
#define BUFFER_OFFSET(i) ((void*)(i))
GLuint create_vbo(const GLsizeiptr size, const GLvoid* data, const GLenum usage);


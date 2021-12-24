#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>


#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

typedef unsigned int uint;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/////解绑缓冲区，会将为当前绑定的缓冲区重置为类似NULL的状态
//void UnBindBuffer();
//
/////解绑顶点数组对象
//void UnBindVertexArrays();

void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
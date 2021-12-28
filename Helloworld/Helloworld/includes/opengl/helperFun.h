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

#define EMPTY_STR ""

#define SUCCESS 0
#define UNKNOWN -1
#define FAILED  -2

///获取数组长度
#define ARRAY_SIZE(vertices) (sizeof(vertices)/sizeof(vertices[0]))
/////得到一个结构体中field所占用的字节数
//#define FSIZ(type,fileld) sizeof(((type*)0)->field)

#define CHAR_POINTER ((char*))

typedef unsigned int uint;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/////解绑缓冲区，会将为当前绑定的缓冲区重置为类似NULL的状态
//void UnBindBuffer();
//
/////解绑顶点数组对象
//void UnBindVertexArrays();

void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
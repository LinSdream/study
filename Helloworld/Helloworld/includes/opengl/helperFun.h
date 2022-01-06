#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>

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

constexpr int SUCCESS = 0x0000;
constexpr int UNKNOWN = 0x0065;
constexpr int FAILED = 0x0066;
constexpr int READ_FILE_FAILED = 0x0067;

///获取数组长度
#define ARRAY_SIZE(vertices) (sizeof(vertices)/sizeof(vertices[0]))
/////得到一个结构体中field所占用的字节数
//#define FSIZ(type,fileld) sizeof(((type*)0)->field)

typedef unsigned int uint;
typedef unsigned char uchar;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::string ReadFile(const char* path);

/////解绑缓冲区，会将为当前绑定的缓冲区重置为类似NULL的状态
//void UnBindBuffer();
//
/////解绑顶点数组对象
//void UnBindVertexArrays();
//
//void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

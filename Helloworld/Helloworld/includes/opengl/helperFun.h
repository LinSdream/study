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
#define NEW_DELEGATE MySpace::NewDelegate

constexpr int SUCCESS = 0x0000;
constexpr int UNKNOWN = 0x0065;
constexpr int FAILED = 0x0066;
constexpr int READ_FILE_FAILED = 0x0067;

///��ȡ���鳤��
#define ARRAY_SIZE(vertices) (sizeof(vertices)/sizeof(vertices[0]))
#define ABS(value) value<0?-value:value;
/////�õ�һ���ṹ����field��ռ�õ��ֽ���
//#define FSIZ(type,fileld) sizeof(((type*)0)->field)

typedef unsigned int uint;
typedef unsigned char uchar;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::string ReadFile(const char* path);

float Clamp01(float value);

float Abs(float value);

/////��󻺳������ὫΪ��ǰ�󶨵Ļ���������Ϊ����NULL��״̬
//void UnBindBuffer();
//
/////��󶥵��������
//void UnBindVertexArrays();
//
//void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

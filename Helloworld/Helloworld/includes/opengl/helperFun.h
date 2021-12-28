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

///��ȡ���鳤��
#define ARRAY_SIZE(vertices) (sizeof(vertices)/sizeof(vertices[0]))
/////�õ�һ���ṹ����field��ռ�õ��ֽ���
//#define FSIZ(type,fileld) sizeof(((type*)0)->field)

#define CHAR_POINTER ((char*))

typedef unsigned int uint;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/////��󻺳������ὫΪ��ǰ�󶨵Ļ���������Ϊ����NULL��״̬
//void UnBindBuffer();
//
/////��󶥵��������
//void UnBindVertexArrays();

void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
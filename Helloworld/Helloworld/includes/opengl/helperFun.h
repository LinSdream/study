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

/////��󻺳������ὫΪ��ǰ�󶨵Ļ���������Ϊ����NULL��״̬
//void UnBindBuffer();
//
/////��󶥵��������
//void UnBindVertexArrays();

void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
#include<opengl/helperFun.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<stdlib.h>
#include<stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
//
//void UnBindBuffer() 
//{
//	//ͨ����0��������ǰ�Ļ�������ΪĬ��ֵ
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//}
//
//void UnBindVertexArrays()
//{
//	//ͨ����0�������������������ΪĬ��ֵ
//	glBindVertexArray(0);
//}
//
///// <summary>
///// ���ö������ݵ�����
///// </summary>
///// <param name="index">���õĶ�������λ��</param>
///// <param name="size">�������ԵĴ�С</param>
///// <param name="type">�������Ե���������</param>
///// <param name="normalized">�������������Ƿ��һ����������ת����[0,1]��</param>
///// <param name="stride">����������������ǽ������еģ�ͨ�����ò�������OpenGL��������ô����</param>
///// <param name="pointer">λ�������ڻ����е���ʼλ�õ�ƫ����</param>
//void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
//{
//	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
//}

std::string ReadFile(const char* path)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		file.open(path);
		if (!file.good())
		{
			std::cout << "Open File Failed: " << path << std::endl;
			file.close();
			return EMPTY_STR;
		}

		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		return stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR:SHADER::FILE_NOT_SUCCESFULLY_READ. IFSTREAM.FAILURE CODE: " << e.code() << ". The Path is" << path << std::endl;
		return EMPTY_STR;
	}
}


float Clamp(float value, float max, float min) 
{
	if (value >= max) return max;
	if (value <= min) return min;
	return value;
}

float Clamp01(float value)
{
	return Clamp(value, 0.0f, 1.0f);
}

float Abs(float value)
{
	return value < 0.0f ? -value : value;
}

GLenum TextureNum(uint num)
{
	return GL_TEXTURE0 + num;
}

char* PathFromat(char* path) 
{
	path = Replace_Str(path, '\\', '/');
	return path;
}

char* Replace_Str(char* str, char ch, char rp) 
{
	uint len = strlen(str);
	char* copy = new char[len + 1];
	for (uint i = 0;i < len;i++)
	{
		if (str[i] == ch) copy[i] = rp;
		else copy[i] = str[i];
	}
	copy[len] = 0;
	strcpy(str, copy);
	delete[] copy;
	return str;
}

void nReplace_Str(char* target, char* dest, char ch, char rp)
{
	uint targetLen = strlen(target);
	uint destLen = strlen(dest);
	if (destLen <= targetLen) throw "Dest lenth to short!Can't replace string";
	dest = { 0 };
	for (uint i = 0;i < targetLen;i++)
	{
		if (target[i] == ch) dest[i] = rp;
		else dest[i] = target[i];
	}
}
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
//	//通过绑定0，来将当前的缓冲区设为默认值
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//}
//
//void UnBindVertexArrays()
//{
//	//通过绑定0，来将定点数组对象设为默认值
//	glBindVertexArray(0);
//}
//
///// <summary>
///// 设置顶点数据的属性
///// </summary>
///// <param name="index">配置的顶点属性位置</param>
///// <param name="size">顶点属性的大小</param>
///// <param name="type">顶点属性的数据类型</param>
///// <param name="normalized">顶点属性数据是否归一化（将数据转化到[0,1]）</param>
///// <param name="stride">步长，数组的数据是紧密排列的，通过设置步长告诉OpenGL的属性怎么设置</param>
///// <param name="pointer">位置数据在缓冲中的起始位置的偏移量</param>
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
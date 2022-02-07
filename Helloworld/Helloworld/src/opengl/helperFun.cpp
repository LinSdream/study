#include<opengl/helperFun.h>
#include<fstream>
#include<sstream>
#include<iostream>

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
		std::cout << "ERROR:SHADER::FILE_NOT_SUCCESFULLY_READ. IFSTREAM.FAILURE CODE: " << e.code() << std::endl;
		return EMPTY_STR;
	}
}

float Clamp01(float value) 
{
	if (value >= 1.0f) return 1.0f;
	if (value <= 0.0f) return 0.0f;
	return value;
}
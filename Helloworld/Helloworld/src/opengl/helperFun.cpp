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
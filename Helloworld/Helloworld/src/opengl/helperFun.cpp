#include<opengl/helperFun.h>

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

/// <summary>
/// ���ö������ݵ�����
/// </summary>
/// <param name="index">���õĶ�������λ��</param>
/// <param name="size">�������ԵĴ�С</param>
/// <param name="type">�������Ե���������</param>
/// <param name="normalized">�������������Ƿ��һ����������ת����[0,1]��</param>
/// <param name="stride">����������������ǽ������еģ�ͨ�����ò�������OpenGL��������ô����</param>
/// <param name="pointer">λ�������ڻ����е���ʼλ�õ�ƫ����</param>
void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}
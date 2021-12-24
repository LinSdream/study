#include<opengl/helperFun.h>

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

/// <summary>
/// 设置顶点数据的属性
/// </summary>
/// <param name="index">配置的顶点属性位置</param>
/// <param name="size">顶点属性的大小</param>
/// <param name="type">顶点属性的数据类型</param>
/// <param name="normalized">顶点属性数据是否归一化（将数据转化到[0,1]）</param>
/// <param name="stride">步长，数组的数据是紧密排列的，通过设置步长告诉OpenGL的属性怎么设置</param>
/// <param name="pointer">位置数据在缓冲中的起始位置的偏移量</param>
void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}
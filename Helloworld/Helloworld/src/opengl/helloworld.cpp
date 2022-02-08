#include <opengl/helloworld.h>
#include<opengl/helperFun.h>
#include<stb/stb_image.h>



void HelloworldGradientEnvironment::Background(GLFWwindow* window)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

DrawTriangle::DrawTriangle(Shader* shader) :DrawBase(shader)
{
	vao_ = new VAOContext();
	vbo_ = new VBOContext();
}

DrawTriangle::~DrawTriangle()
{
	delete vao_;
	delete vbo_;
}

void DrawTriangle::Init(const void* vertices, int size)
{
	vao_->Bind();
	vbo_->Bind();

	//float vertices[] = {
	//	//λ��				//��ɫ
	//	-0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f,// left  
	//	0.1f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
	//	0.0f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f,// top   
	//};

	vbo_->SetBufferData(size, vertices, GL_STATIC_DRAW);

	//�ο��ĵ�<https://learnopengl-cn.github.io/01%20Getting%20started/05%20Shaders/#_5>
	//����˵���Ǵ��ݽ�ȥ������ֻ��һ��һά�����ݣ���ʵ������£����������ж��غ���ģ����Ծ���Ҫȥָ�����͸������ݵĺ���
	//����λ����Ϣ
	//��ʼλ��Ϊ0��ÿ3������Ϊһ�����ݣ���һ��Ŀ�ʼ�Ǵ�0��ʼ������ƫ��6��float���ȵ�����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//������ɫ��Ϣ
	//��ʼλ��Ϊ0��ÿ3������Ϊһ�����ݣ���һ��Ŀ�ʼ�Ǵ�3��float֮���λ�ã�����ƫ��6��float���ȵ�����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//������ΪĬ��ֵ,���ں�����Draw�����½��а󶨣�������ʵ���Բ��ã�����Ϊ���Ͻ����ﻹ�ǽ���һ�ε�����
	//ͨ����0��������ǰ�Ļ�������ΪĬ��ֵ
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//ͨ����0�������������������ΪĬ��ֵ
	glBindVertexArray(0);
}

void DrawTriangle::Draw(const void* context)
{
	shader_->Use();
	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

DrawRectangle::DrawRectangle(Shader* shader):DrawBase(shader)
{
	vao_ = new VAOContext();
	ebo_ = new EBOContext();
	vbo_ = new VBOContext();

	glGenTextures(1, &texture1_);
	glGenTextures(1, &texture2_);

	visibilityValue_ = 0.0f;
}

DrawRectangle::~DrawRectangle() 
{
	glDeleteTextures(1, &texture1_);
	glDeleteTextures(1, &texture2_);

	delete vao_;
	delete vbo_;
	delete ebo_;
}

void DrawRectangle::Init(const void* vertices,int size) 
{

	vao_->Bind();
	vbo_->Bind();

	//glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	//vbo_->SetBufferData(size, vertices, GL_STATIC_DRAW);
	ebo_->Bind();

	//���������εĶ���λ��
	uint indices[] = {
		0,1,3, //��һ��������
		1,2,3, //�ڶ���������
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);

	//����������openGL,���������һ��2d������PS:2d���������Ϊs t
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//��������ͼƬ
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	uchar* image = stbi_load("./assets/textures/container.jpg", &width, &height, &nrChannels, 0);

	if (image) 
	{
		//�ڵ�ǰ�İ�����������ڻ����Ԫ��������
		//glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
		//target: ָ�������Ŀ�����ͣ�GL_TEXTURE_1D,GL_TEDTURE_2D,GL_TEXTURE_3D
		//level:  ָ���༶��Զ����ļ���0Ϊ����ͼ�񼶱�n��ʾ��n��mipmap����СͼƬ
		//internalFormat: ָ��������ĸ�ʽ������ͻ��ֻ��RGB�ʹ洢ΪRGBֵ��
		//width:  ָ������Ŀ��
		//height: ָ������ͼƬ�ĸ߶ȣ��������������еĲ���
		//border: ����Ϊ0��api����ʷ��������
		//format: ָ��ԭͼ�����ݸ�ʽ������ԭͼֻ��RGB,���������RGB,���������alphaͨ������Ҫ��pngͼƬ�������GL_RGBA	
		//type:   ָ��ԭͼ����������
		//data:	  ͼƬ����ָ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		//��������
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//�������������úú󣬼ǵ�Ҫ�ͷ�
	stbi_image_free(image);

	//glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image = stbi_load("./assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//�������������úú󣬼ǵ�Ҫ�ͷ�
	stbi_image_free(image);

	shader_->Use();
	shader_->SetInt("ourTexture1", 0);
	shader_->SetInt("ourTexture2", 1);
}

void DrawRectangle::Draw(const void* context)
{

	//����ж��������Ҫ��openGL���ж��壬�������ĸ�������Ԫ�������һ����������Ҫ��openGL��������16��,0-15
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	shader_->Use();
	//shader_->SetInt("ourTexture1", 0);
	//shader_->SetInt("ourTexture2", 1);

	float time = glfwGetTime();

	shader_->Set2f("towards", -1.0f, -1.0f);
	Context* c= (Context*)context;
	if (glfwGetKey(c->window_, GLFW_KEY_UP) == GLFW_PRESS) {
		c->visibilityValue_ += 0.1f;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_DOWN) == GLFW_PRESS) {
		c->visibilityValue_ -= 0.1f;
	}

	shader_->SetFloat("visibility", Clamp01(c->visibilityValue_));
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

	trans = glm::rotate(trans, time, glm::vec3(0, 0, 1.0f));
	shader_->SetMatrix4fv("transform", 1, GL_FALSE, glm::value_ptr(trans));

	vao_->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
	float value = sin(time);
	trans = glm::scale(trans, glm::vec3(1.0f,1.0f, 1.0f) * Abs(value));
	trans = glm::rotate(trans, time, glm::vec3(0.0f, 1.0f, 0.0f));
	shader_->SetMatrix4fv("transform", 1, GL_FALSE, glm::value_ptr(trans));

	vao_->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

DrawTwoConnectTriangle::DrawTwoConnectTriangle(Shader* shader):DrawBase(shader)
{
	vao_ = new VAOContext();
	vbo_ = new VBOContext();
}

DrawTwoConnectTriangle::~DrawTwoConnectTriangle() 
{
	//ģʽѡ�����(Ĭ�ϣ������������)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	delete vao_;
	delete vbo_;
}

void DrawTwoConnectTriangle::Init() 
{
	vao_->Bind();
	vbo_->Bind();

	float vertices[] =
	{
		-1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,1.0f, 0.0f, 1.0f,
		0.5f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,0.0f, 1.0f, 0.0f,
		0.5f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f,
	};

	vbo_->SetBufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//ģʽѡ���߿�
	//�ú�����������OpenGL��λ���ͼԪ
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void DrawTwoConnectTriangle::Draw(const void* context)
{
	shader_->Use();
	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 9);
}

DrawTwoTriangleUseDiffVAOandVBO::DrawTwoTriangleUseDiffVAOandVBO(Shader* shader):DrawBase(shader)
{
	glGenBuffers(2, vbos_);
	glGenVertexArrays(2, vaos_);
}

DrawTwoTriangleUseDiffVAOandVBO::~DrawTwoTriangleUseDiffVAOandVBO() 
{
	glDeleteBuffers(2, vbos_);
	glDeleteVertexArrays(2, vaos_);
}

void DrawTwoTriangleUseDiffVAOandVBO::Init(const void* vertices,int size)
{

	//float vertices[] = {
	//	//λ��				//��ɫ
	//	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// left  
	//	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
	//	0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// top   
	//};

	float secondVertices[] = {
		0.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f,// left
		0.5f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f, // right 
		1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,// top 
	};
	glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
	glBindVertexArray(vaos_[0]);

	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
	glBindVertexArray(vaos_[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(secondVertices), secondVertices, GL_DYNAMIC_DRAW);
	//because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void DrawTwoTriangleUseDiffVAOandVBO::Draw(const void* context)
{
	shader_->Use();
	glBindVertexArray(vaos_[0]);

	shader_->SetBoolean("useOffset", true);
	float time = glfwGetTime();
	float valueX = sin(time);
	float valueY = cos(time);
	shader_->Set3f("aPosXYZ", valueX, valueY, 0.0f);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vaos_[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

DrawDynamicTriangle::DrawDynamicTriangle(Shader* shader):DrawBase(shader)
{
	vao_ = new VAOContext();
	vbo_ = new VBOContext();

}

DrawDynamicTriangle::~DrawDynamicTriangle()
{
	delete vao_;
	delete vbo_;
}

void DrawDynamicTriangle::Init() 
{

}

void DrawDynamicTriangle::Draw(const void* context)
{
	vao_->Bind();
	vbo_->Bind();

	float vertices[18] = {
		//λ��				//��ɫ
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// left  
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// top  
	};
	
	float time = glfwGetTime();
	float cosValue = cos(time);
	float sinValue = sin(time);

	int index = 0;
	vertices[index] = sinValue;
	vertices[index + 1] = cosValue;

	index += 6;
	vertices[index] = sinValue * 0.5f;
	vertices[index + 1] = cosValue * 0.5f;

	index += 6;

	vertices[index] = 0 - cosValue;
	vertices[index + 1] = 0 - cosValue ;


	index = 3;

	vertices[index] = sinValue;
	vertices[index + 1] = cosValue;
	vertices[index + 2] = cosValue;

	index += 6;
	vertices[index] = sinValue * 0.5f;
	vertices[index + 1] = cosValue * 0.5f;
	vertices[index + 2] = cosValue * 0.5f;

	index += 6;

	vertices[index] = 0 - sinValue;
	vertices[index + 1] = 0 - cosValue;
	vertices[index + 2] = 0 - cosValue;

	vbo_->SetBufferData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	vbo_->Bind();

	shader_->Use();

	float valueX = sin(time);
	float valueY = cos(time);

	shader_->SetBoolean("useOffset", true);
	shader_->Set3f("aPosXYZ", valueX, valueY, 0.0f);

	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


Draw3D::Draw3D(Shader* shader) :DrawBase(shader)
{
	vao_ = new VAOContext();
	ebo_ = new EBOContext();
	vbo_ = new VBOContext();

	glGenTextures(1, &texture1_);
	glGenTextures(1, &texture2_);
}

Draw3D::~Draw3D()
{
	glDeleteTextures(1, &texture1_);
	glDeleteTextures(1, &texture2_);

	delete vao_;
	delete ebo_;
	delete vbo_;
}

void Draw3D::Init(const void* vertices, int size)
{
	vao_->Bind();
	vbo_->Bind();

	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);

	ebo_->Bind();

	uint indices[] =
	{
		0,1,3,
		1,2,3,
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//������
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	uchar* image = stbi_load("./assets/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, texture2_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image = stbi_load("./assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	shader_->Use();
	shader_->SetInt("ourTexture1", 0);
	shader_->SetInt("ourTexture2", 1);
}

void Draw3D::Draw(const void* context)
{

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2_);

	shader_->Use();
	float time = glfwGetTime();

	shader_->Set2f("towards", -1.0f, -1.0f);
	Context* c = (Context*)context;
	if (glfwGetKey(c->window_, GLFW_KEY_UP) == GLFW_PRESS) {
		c->visibilityValue_ += 0.1f;
	}
	if (glfwGetKey(c->window_, GLFW_KEY_DOWN) == GLFW_PRESS) {
		c->visibilityValue_ -= 0.1f;
	}

	shader_->SetFloat("visibility", Clamp01(c->visibilityValue_));
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

	trans = glm::rotate(trans, time, glm::vec3(0, 0, 1.0f));
	shader_->SetMatrix4fv("transform", 1, GL_FALSE, glm::value_ptr(trans));

	vao_->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}
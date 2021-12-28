#include<iostream>

#include <opengl/helloworld.h>

void HelloworldEnvironment::Background(GLFWwindow *window)
{

}

void HelloworldEnvironment::PressInput(GLFWwindow *window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void HellowworldGradientEnvironment::Background(GLFWwindow* window)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

ShaderContext::ShaderContext()
{
	shaderProgram_ = glCreateProgram();
}

bool ShaderContext::Link(uint** shaders, int size) 
{
	if (shaderProgram_ == NULL) return false;
	for (int i = 0; i < size; i++) 
	{
		glAttachShader(shaderProgram_, *(shaders[i]));
	}

	glLinkProgram(shaderProgram_);

	int success;
	glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
	if (!success) 
	{
		char log[512];
		glGetProgramInfoLog(shaderProgram_, 512, NULL, log);
		std::cout << "Link Shader Program Failed.Info:" << log << std::endl;
		return false;
	}
	for (int i = 0; i < size; i++) 
	{
		glDeleteShader(*(shaders[i]));
	}
	return true;
}

void ShaderContext::UseProgram() 
{
	if (shaderProgram_ == NULL) return;

	glUseProgram(shaderProgram_);
}

bool ShaderContext::CreateShader(uint* shader,GLenum type, int shaderSouceSize, char* shaderSource) 
{
	*shader = glCreateShader(type);
	glShaderSource(*shader, shaderSouceSize, &shaderSource, NULL);
	glCompileShader(*shader);

	int success;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
	if(!success) 
	{
		char log[512];
		glGetShaderInfoLog(*shader, 512, NULL, log);
		std::cout << "Create Shader Failed.Shader Type Code:" << type << "Shader Source:" << shaderSource << std::endl;
		return false;
	}
	return true;
}

VBOContext::VBOContext()
{
	glGenBuffers(1, &vbo_);
}

VBOContext::~VBOContext() 
{
	glDeleteBuffers(1, &vbo_);
}

void VBOContext::SetBufferData(GLsizeiptr size, const void* data, GLenum usage)
{
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VBOContext::Bind() 
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
}

VAOContext::VAOContext()
{
	glGenVertexArrays(1, &vao_);
}

VAOContext::~VAOContext() 
{
	glDeleteVertexArrays(1, &vao_);
}

void VAOContext::Bind() 
{
	glBindVertexArray(vao_);
}

EBOContext::EBOContext() 
{
	glGenBuffers(1, &ebo_);
}

EBOContext::~EBOContext() 
{
	glDeleteBuffers(1, &ebo_);
}

void EBOContext::Bind() 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
}

DrawTriangle::DrawTriangle() :DrawBase()
{
	vao_ = new VAOContext();
	vbo_ = new VBOContext();

}

DrawTriangle::~DrawTriangle()
{
	delete vao_;
	delete vbo_;
}

void DrawTriangle::Init()
{
	vao_->Bind();
	vbo_->Bind();

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};

	vbo_->SetBufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//先设置为默认值,由于后面在Draw有重新进行绑定，所以其实可以不用，但是为了严谨这里还是进行一次的设置
	//通过绑定0，来将当前的缓冲区设为默认值
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//通过绑定0，来将定点数组对象设为默认值
	glBindVertexArray(0);
}

void DrawTriangle::Draw() 
{
	vao_->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

DrawRectangle::DrawRectangle()
{
	vao_ = new VAOContext();
	ebo_ = new EBOContext();
	vbo_ = new VBOContext();
}

DrawRectangle::~DrawRectangle() 
{
	delete vao_;
	delete vbo_;
	delete ebo_;
}

void DrawRectangle::Init() 
{
	vao_->Bind();
	vbo_->Bind();

	float rectangleVertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};

	//glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

	vbo_->SetBufferData(sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
	ebo_->Bind();

	//两个三角形的顶点位置
	uint indices[] = {
		0,1,3, //第一个三角形
		1,2,3, //第二个三角形
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void DrawRectangle::Draw() 
{
	vao_->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

DrawTwoConnectTriangle::DrawTwoConnectTriangle() 
{
	vao_ = new VAOContext();
	vbo_ = new VBOContext();
}

DrawTwoConnectTriangle::~DrawTwoConnectTriangle() 
{
	//模式选择，填充(默认，这里就是重置)
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
		-1.0f, 0.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.5f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,
	};

	vbo_->SetBufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//模式选择线框
	//该函数用于配置OpenGL如何绘制图元
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void DrawTwoConnectTriangle::Draw() 
{
	vao_->Bind();
	
	glDrawArrays(GL_TRIANGLES, 0, 9);
}

DrawTwoTriangleUseDiffVAOandVBO::DrawTwoTriangleUseDiffVAOandVBO() 
{
	glGenBuffers(2, vbos_);
	glGenVertexArrays(2, vaos_);
}

DrawTwoTriangleUseDiffVAOandVBO::~DrawTwoTriangleUseDiffVAOandVBO() 
{
	glDeleteBuffers(2, vbos_);
	glDeleteVertexArrays(2, vaos_);
}

void DrawTwoTriangleUseDiffVAOandVBO::Init() 
{

	float vertices[] = {
		-1.0f, 0.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	float secondVertices[] = {
		0.0f, 0.0f, 0.0f,
		0.5f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
	glBindVertexArray(vaos_[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
	glBindVertexArray(vaos_[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(secondVertices), secondVertices, GL_STATIC_DRAW);
	//because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);


}

void DrawTwoTriangleUseDiffVAOandVBO::Draw() 
{
	glBindVertexArray(vaos_[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vaos_[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Shader::Shader(char* vertex,char* fragment) 
{
	programID_ = glCreateProgram();

	code_ = SUCCESS;

	vertexShader_=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_, 1, &vertex, NULL);
	glCompileShader(vertexShader_);
	int success;
	glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		char log[512];
		glGetShaderInfoLog(vertexShader_, 512, NULL, log);		
		std::cout << "Create GL_VERTEX_SHADER Failed.Code:" << success << "Shader Source:" << vertex << std::endl;
		code_ = CREATE_VERTEX_SHADER_FAILED;
		return;
	}

	fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_, 1, &fragment, NULL);
	glCompileShader(fragmentShader_);
	glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(fragmentShader_, 512, NULL, log);
		std::cout << "Create GL_FRAGMENT_SHADER Failed.Code:" << success << "Shader Source:" << fragment << std::endl;
		code_ = CREATE_VERTEX_SHADER_FAILED;
		return;
	}
}

Shader::~Shader() 
{
	glDeleteShader(programID_);
}


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<windows/window.h>
#include<opengl/helloworld.h>
#include<opengl/shaderReader.h>

class Context;

void Update(GLFWwindow* window, void* context);
Context* CreateContext();
void DestroyContext(Context* context);

class Context
{
public:

	HelloworldEnvironment* env_;
	ShaderContext* shader1_;
	ShaderContext* shader2_;
	DrawBase* draw1_;
	DrawBase* draw2_;

};

// api documents:<https://www.khronos.org/registry/OpenGL-Refpages/gl4/>
int main() 
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	Window* window = new Window(800, 600, (char*)"hello world", [](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height); });
	int code = window->GetInitializeStatus();
	if (code != (int)WINDOWSTATUS_SUCCESS) 
	{
		delete window;
		return code;
	}
	
	Context* context = CreateContext();
	window->Bind(context);

	uint vertexShader, fragmentShader, fragmentShader2;

	ShaderReader reader;

	//坑点：
	//不能在类对象内部调用c_str()方法，因为连一块写的话，会产生一个临时变量存住string的内容，然后这个string就会被销毁，导致内容丢失.
	//因此需要显性的有个temp变量hold住字符串内容，然后在调用c_str()
	std::string temp = reader.Read("../Shaders/helloworld.vs");
	if (temp.empty()) return FAILED;
	
	context->shader1_->CreateShader(&vertexShader, GL_VERTEX_SHADER, 1, (char*)temp.c_str());

	temp = reader.Read("../Shaders/helloworld.fs");
	if (temp.empty()) return FAILED;

	context->shader1_->CreateShader(&fragmentShader, GL_FRAGMENT_SHADER, 1, (char*)temp.c_str());
	context->shader2_->CreateShader(&fragmentShader2, GL_FRAGMENT_SHADER, 1, (char*)temp.c_str());
	
	uint* shaders[2] = {
		&vertexShader,
		&fragmentShader
	};
	
	uint* shader[2] =
	{
		&vertexShader,
		&fragmentShader2,
	};

	if (!context->shader1_->Link(shaders, 2)) 
	{
		DestroyContext(context);
		delete window;

		return FAILED;
	}

	if (!context->shader2_->Link(shader, 2)) 
	{
		DestroyContext(context);
		delete window;

		return FAILED;
	}

	context->draw1_->Init();
	context->draw2_->Init();
	window->Update(Update);
	window->UnBind();

	DestroyContext(context);
	delete window;

	return SUCCESS;

}

void Update(GLFWwindow* window,void*context) 
{
	Context *c= (Context*)context;

	c->env_->Background(window);
	c->env_->PressInput(window);

	c->shader1_->UseProgram();

	float time = glfwGetTime();
	float greenValue = sin(time) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(c->shader1_->GetProgramID(), "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	c->draw1_->Draw();

	c->shader2_->UseProgram();

	vertexColorLocation = glGetUniformLocation(c->shader2_->GetProgramID(), "ourColor");
	glUniform4f(vertexColorLocation, 0.5f, 0.2f, 0.9f, 1.0f);

	c->draw2_->Draw();
}

Context* CreateContext()
{
	
	Context* context = new Context();

	context->env_ = new HellowworldGradientEnvironment();
	context->shader1_ = new ShaderContext();
	context->shader2_ = new ShaderContext();

	//context->draw_ = new DrawTriangle();
	context->draw1_ = new DrawTwoTriangleUseDiffVAOandVBO();
	context->draw2_ = new DrawTriangle();
	return context;
}

void DestroyContext(Context* context) 
{

	delete context->env_;
	delete context->shader1_;
	delete context->shader2_;

	delete context->draw1_;
	delete context->draw2_;
	delete context;
}
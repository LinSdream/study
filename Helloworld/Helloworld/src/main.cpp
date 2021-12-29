#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<windows/window.h>
#include<opengl/helloworld.h>

class Context;

void Update(GLFWwindow* window, void* context);
Context* CreateContext(int* code);
void DestroyContext(Context* context);

class Context
{
public:

	HelloworldEnvironment* env_;
	Shader* shader_;
	DrawBase* draw1_;
	DrawBase* draw2_;

};

// api documents:<https://www.khronos.org/registry/OpenGL-Refpages/gl4/>
int main() 
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	Window* window = new Window(800, 600, (char*)"hello world",
		[](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height); });

	int code = window->GetInitializeStatus();
	if (code != (int)WINDOWSTATUS_SUCCESS) 
	{
		delete window;
		return code;
	}
	
	Context* context = CreateContext(&code);
	if (code != SUCCESS) 
	{
		delete window;
		return code;
	}

	window->Bind(context);

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

	c->draw1_->Draw([](Shader* shader) {
		shader->Use();
		float time = glfwGetTime();
		float greenValue = sin(time) / 2.0f + 0.5f;
		shader->Set4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		});


	c->draw2_->Draw([](Shader* shader) {
		shader->Use();
		shader->Set4f("ourColor", 0.5f, 0.2f, 0.9f, 1.0f);
		});
}

Context* CreateContext(int* code)
{

	//坑点：
	//不能在类对象内部调用c_str()方法，因为连一块写的话，会产生一个临时变量存住string的内容，然后这个string就会被销毁，导致内容丢失.
	//因此需要显性的有个temp变量hold住字符串内容，然后在调用c_str()
	std::string vs = ReadFile("../Shaders/helloworld.vs");
	if (vs.empty()) 
	{
		*code = FAILED;
		return NULL;
	}

	std::string fs = ReadFile("../Shaders/helloworld.fs");
	if (fs.empty()) 
	{
		*code = FAILED;
		return NULL;
	}

	Context* context = new Context();
	context->shader_ = new Shader((char*)vs.c_str(), (char*)fs.c_str());
	if (context->shader_->InitiationStatus() != SUCCESS)
	{
		*code = context->shader_->InitiationStatus();
		delete context->shader_;
		delete context;
		return NULL;
	}

	context->env_ = new HelloworldGradientEnvironment();
	context->draw1_ = new DrawTwoTriangleUseDiffVAOandVBO(context->shader_);
	context->draw2_ = new DrawTriangle(context->shader_);

	*code = SUCCESS;

	return context;
}

void DestroyContext(Context* context) 
{

	delete context->env_;
	delete context->shader_;

	delete context->draw1_;
	delete context->draw2_;
	delete context;
}
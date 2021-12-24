#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<windows/window.h>
#include<opengl/helloworld.h>

const char* VERTEX_SHADER_SOURCE="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* FRAGMENT_SHADER_SOURCE="#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

const char* FRAGMENT_SHADER_SOURCE_SECOND = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.5f, 0.2f, 0.9f, 1.0f);\n"
"}\n\0";

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

	context->shader1_->CreateShader(&vertexShader, GL_VERTEX_SHADER, 1, (char*)VERTEX_SHADER_SOURCE);
	context->shader1_->CreateShader(&fragmentShader, GL_FRAGMENT_SHADER, 1, (char*)FRAGMENT_SHADER_SOURCE);
	context->shader2_->CreateShader(&fragmentShader2, GL_FRAGMENT_SHADER, 1, (char*)FRAGMENT_SHADER_SOURCE_SECOND);
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
	c->draw1_->Draw();

	c->shader2_->UseProgram();
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
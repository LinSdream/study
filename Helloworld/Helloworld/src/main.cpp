#include<windows/window.h>
#include<opengl/helloworld.h>

class Context;

void Update(GLFWwindow* window, void* context);
Context* CreateContext(int* code);
void DestroyContext(Context* context);



// api documents:<https://www.khronos.org/registry/OpenGL-Refpages/gl4/>
int main() 
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	Window* window = new Window(800, 600, (char*)"hello world",
		[](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height); });

	int code = window->GetInitializeStatus();
	if (code != (int)EWindowStatus::WINDOWSTATUS_SUCCESS)
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

	float vertices[] = {
		//位置				//颜色
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// left  
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// top   
	};

	float rectangleVertices[] = {

		////顶点位置				//颜色				// 纹理坐标
		//0.5f, 0.5f, 0.0f,		1.0f,0.0f,0.0f,		2.0f, 2.0f,		0.3f, 0.3f,// 右上角
		//0.5f, -0.5f, 0.0f,		0.0f,1.0f,0.0f,		2.0f, 0.0f,		0.3f, 0.0f,// 右下角
		//-0.5f, -0.5f, 0.0f,		0.0f,0.0f,1.0f,		0.0f, 0.0f,		0.0f, 0.0f,// 左下角
		//-0.5f, 0.5f, 0.0f,		1.0f,1.0f,0.0f,		0.0f, 2.0f,		0.0f, 0.1f,// 左上角

		//顶点位置				//颜色				// 纹理坐标
		0.5f, 0.5f, 0.0f,		1.0f,0.0f,0.0f,		2.0f, 2.0f,		1.0f, 1.0f,// 右上角
		0.5f, -0.5f, 0.0f,		0.0f,1.0f,0.0f,		2.0f, 0.0f,		1.0f, 0.0f,// 右下角
		-0.5f, -0.5f, 0.0f,		0.0f,0.0f,1.0f,		0.0f, 0.0f,		0.0f, 0.0f,// 左下角
		-0.5f, 0.5f, 0.0f,		1.0f,1.0f,0.0f,		0.0f, 2.0f,		0.0f, 1.0f,// 左上角
	};

	context->draw1_->Init(vertices,sizeof(vertices));
	context->draw2_->Init(rectangleVertices, sizeof(rectangleVertices));

	window->Update(Update);
	window->UnBind();

	DestroyContext(context);
	delete window;

	return SUCCESS;

}

void Update(GLFWwindow* window,void*context) 
{
	Context *c= (Context*)context;
	c->window_ = window;

	c->env_->Background(window);
	c->env_->PressInput(window);

	float time = glfwGetTime();

	c->draw2_->Draw(context);
	c->draw1_->Draw(context);
	
}

Context* CreateContext(int* code)
{
	Context* context = new Context();
	context->shaderManager_ = new ShadersManager();
	
	ShadersManager* sm = context->shaderManager_;

	//这里需要判定，懒得写了
	sm->CreateShader("helloWorld", "../Shaders/helloworld.vs", "../Shaders/helloworld.fs");
	sm->CreateShader("texture", "../Shaders/textures.vs", "../Shaders/textures.fs");

	context->env_ = new HelloworldGradientEnvironment();
	context->draw1_ = new DrawTwoTriangleUseDiffVAOandVBO((*sm)["helloWorld"]);
	context->draw2_ = new Draw3D((*sm)["texture"]);

	*code = SUCCESS;

	return context;
}

void DestroyContext(Context* context) 
{
	if (!context) return;
	if (context->env_) delete context->env_;
	if (context->draw1_) delete context->draw1_;
	if (context->draw2_) delete context->draw2_;
	if (context->shaderManager_) delete context->shaderManager_;
	delete context;
}
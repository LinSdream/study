#include<windows/window.h>
#include<opengl/helloworld.h>

void Update(Window* w,GLFWwindow* window, double delaTime,void* context);
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

	window->RegisterMousePosition_Callback([](GLFWwindow* window, double posX, double posY) 
		{
			void* context = glfwGetWindowUserPointer(window);
			Context* c = static_cast<Context*>(context);
			c->draws_[2]->MoveCamera(posX, posY);
		});

	//window->RegisterMouseScroll_Callback([](GLFWwindow* window, double xoffset, double yoffset) 
	//	{
	//		void* context = glfwGetWindowUserPointer(window);
	//		Context* c = static_cast<Context*>(context);
	//	});

	int count = context->DrawsCount();
	for (int i = 0; i < count; i++)
	{
		context->draws_[i]->Init(window);
	}

	window->Update(Update);
	window->UnBind();

	DestroyContext(context);
	delete window;

	return SUCCESS;
}

void Update(Window* w,GLFWwindow* window, double delaTime,void*context)
{
	Context *c= (Context*)context;
	
	DrawContext drawContext;
	drawContext.window_ = window;
	drawContext.windowHeight_ = w->GetHeight();
	drawContext.windowWidth_ = w->GetWeight();
	drawContext.delaTime_ = delaTime;

	c->env_->Background(window);
	c->env_->PressInput(window);

	float time = glfwGetTime();
	int count = c->DrawsCount();
	c->draws_[2]->Draw(&drawContext);
	//for (int i = 0; i < count; i++)
	//{
	//	c->draws_[i]->Draw(&drawContext);
	//}
}

Context* CreateContext(int* code)
{

	Context* context = new Context();
	context->shaderManager_ = new ShadersManager();
	context->camera_ = new FPS_Camera(glm::vec3(0.0f,3.0f, 3.0f));
	ShadersManager* sm = context->shaderManager_;

	//这里需要判定，懒得写了
	sm->CreateShader("helloWorld", "../Shaders/helloworld.vs", "../Shaders/helloworld.fs");
	sm->CreateShader("texture", "../Shaders/textures.vs", "../Shaders/textures.fs");
	sm->CreateShader("hello3D", "../Shaders/hello3d.vs", "../Shaders/textures.fs");
	sm->CreateShader("light", "../Shaders/light.vs", "../Shaders/light.fs");
	sm->CreateShader("light_cube","../Shaders/light_cube.vs", "../Shaders/light_cube.fs");

	context->env_ = new HelloworldGradientEnvironment();

	context->InitDraws(3);

	context->draws_[0] = new DrawTwoTriangleUseDiffVAOandVBO((*sm)["helloWorld"]);
	context->draws_[1] = new DrawCamera((*sm)["hello3D"], context->camera_);
	context->draws_[2] = new LightDraw((*sm)["light"], (*sm)["light_cube"], context->camera_);

	*code = SUCCESS;

	return context;
}

void DestroyContext(Context* context) 
{
	if (!context) return;
	if (context->env_) delete context->env_;

	context->RecycleDraws();
	if (context->shaderManager_) delete context->shaderManager_;
	delete context;
}


#include<windows/window.h>
#include<opengl/helloworld.h>

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

	window->RegisterMouseScroll_Callback([](GLFWwindow* window, double xpos, double ypos) {
			void* context = glfwGetWindowUserPointer(window);
			Context* c = static_cast<Context*>(context);
			c->draws_[2]->CameraScroll(static_cast<float>(ypos));
		});

	int count = context->DrawsCount();
	for (int i = 0; i < count; i++)
	{
		context->draws_[i]->Init(window);
	}

	double deltaTime = 0.0;
	double lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		Context* c = (Context*)context;

		float time = static_cast<float>(glfwGetTime());

		DrawContext drawContext;
		window->GetSize(&(drawContext.windowWidth_), &(drawContext.windowHeight_));
		drawContext.aspectRatio_ = (drawContext.windowWidth_ * 1.0f) / (drawContext.windowHeight_ * 1.0f);
		drawContext.delaTime_ = deltaTime;
		drawContext.time_ = time;

		c->env_->Background(window->GetWindow());
		c->env_->PressInput(window->GetWindow());

		int count = c->DrawsCount();

		c->draws_[2]->InputDrive(window->GetWindow(), time, deltaTime);

		c->draws_[2]->Update(&drawContext);

		glfwSwapBuffers(window->GetWindow());
		glfwPollEvents();
	}

	window->UnBind();

	DestroyContext(context);
	delete window;

	return SUCCESS;
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
	sm->CreateShader("lightmap","../Shaders/lightmap.vs", "../Shaders/lightmap.fs");
	sm->CreateShader("lightcasters_DirectionalLight","../Shaders/lightmap.vs", "../Shaders/lightcasters_DirectionalLight.fs");
	sm->CreateShader("lightcasters_PointLight","../Shaders/lightmap.vs", "../Shaders/lightcasters_PointLight.fs");
	sm->CreateShader("lightcasters_Spotlight","../Shaders/lightmap.vs", "../Shaders/lightcasters_Spotlight.fs");

	context->env_ = new HelloworldGradientEnvironment();

	context->InitDraws(7);

	context->draws_[0] = new DrawTwoTriangleUseDiffVAOandVBO((*sm)["helloWorld"]);
	context->draws_[1] = new DrawCamera((*sm)["hello3D"], context->camera_);
	context->draws_[6] = new Lightcasters_Spotlight_Draw((*sm)["lightcasters_Spotlight"], (*sm)["light_cube"], context->camera_);
	context->draws_[3] = new LightDraw((*sm)["light"], (*sm)["light_cube"], context->camera_);
	context->draws_[4] = new LightingMapsDraw((*sm)["lightmap"], (*sm)["light_cube"], context->camera_);
	context->draws_[5] = new Lightcasters_DirectionalLight_Draw((*sm)["lightcasters_DirectionalLight"], (*sm)["light_cube"], context->camera_);
	context->draws_[2] = new Lightcasters_PointLight_Draw((*sm)["lightcasters_PointLight"], (*sm)["light_cube"], context->camera_);

	*code = SUCCESS;

	return context;
}

void DestroyContext(Context* context) 
{
	if (!context) return;
	if (context->env_) delete context->env_;
	
	context->RecycleDraws();

	if (context->camera_) delete context->camera_;

	if (context->shaderManager_) delete context->shaderManager_;

	delete context;
}


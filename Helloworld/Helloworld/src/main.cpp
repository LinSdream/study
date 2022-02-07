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
	DrawBase* draw1_;
	DrawBase* draw2_;
	ShadersManager* shaderManager_;
	float visibilityValue_;
};


void Say() 
{
	printf("!!!!!!!!!");
}

class A 
{
public:
	void Say() { printf("AAAAAAAAAAAA"); }
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
		//λ��				//��ɫ
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// left  
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right 
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// top   
	};

	float rectangleVertices[] = {

		////����λ��				//��ɫ				// ��������
		//0.5f, 0.5f, 0.0f,		1.0f,0.0f,0.0f,		2.0f, 2.0f,		0.3f, 0.3f,// ���Ͻ�
		//0.5f, -0.5f, 0.0f,		0.0f,1.0f,0.0f,		2.0f, 0.0f,		0.3f, 0.0f,// ���½�
		//-0.5f, -0.5f, 0.0f,		0.0f,0.0f,1.0f,		0.0f, 0.0f,		0.0f, 0.0f,// ���½�
		//-0.5f, 0.5f, 0.0f,		1.0f,1.0f,0.0f,		0.0f, 2.0f,		0.0f, 0.1f,// ���Ͻ�

		//����λ��				//��ɫ				// ��������
		0.5f, 0.5f, 0.0f,		1.0f,0.0f,0.0f,		2.0f, 2.0f,		1.0f, 1.0f,// ���Ͻ�
		0.5f, -0.5f, 0.0f,		0.0f,1.0f,0.0f,		2.0f, 0.0f,		1.0f, 0.0f,// ���½�
		-0.5f, -0.5f, 0.0f,		0.0f,0.0f,1.0f,		0.0f, 0.0f,		0.0f, 0.0f,// ���½�
		-0.5f, 0.5f, 0.0f,		1.0f,1.0f,0.0f,		0.0f, 2.0f,		0.0f, 1.0f,// ���Ͻ�
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

	c->env_->Background(window);
	c->env_->PressInput(window);

	c->draw2_->Draw([&](Shader* shader) -> void {
		//�����˿��������򣬿���ȥ��ϰ�ߴ��ˣ�����
		shader->Set2f("towards", -1.0f, -1.0f);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			c->visibilityValue_ += 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			c->visibilityValue_ -= 0.1f;
		}

		shader->SetFloat("visibility", Clamp01(c->visibilityValue_));
		});

	c->draw1_->Draw([&](Shader* shader) {
		shader->SetBoolean("useOffset", true);
		float time = glfwGetTime();
		float valueX = sin(time);
		float valueY = cos(time);
		shader->Set3f("aPosXYZ", valueX, valueY, 0.0f);
		});
	
}

Context* CreateContext(int* code)
{
	Context* context = new Context();
	context->shaderManager_ = new ShadersManager();
	
	ShadersManager* sm = context->shaderManager_;

	//������Ҫ�ж�������д��
	sm->CreateShader("helloWorld", "../Shaders/helloworld.vs", "../Shaders/helloworld.fs");
	sm->CreateShader("texture", "../Shaders/textures.vs", "../Shaders/textures.fs");

	context->env_ = new HelloworldGradientEnvironment();
	context->draw1_ = new DrawTwoTriangleUseDiffVAOandVBO((*sm)["helloWorld"]);
	context->draw2_ = new DrawRectangle((*sm)["texture"]);

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
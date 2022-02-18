#include<windows/window.h>

Window::Window(int width, int height, char* title, FramebuffSizeCallback callback)
{
	init_code_ = (int)WINDOWSTATUS_NO_INIT;
	context_ = NULL;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	window_ = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window_ == NULL) 
	{
		init_code_ = (int)WINDOWSTATUS_CREATRE_WINDOW;
		return;
	}

	glfwMakeContextCurrent(window_);
	if (callback != NULL)
	{
		glfwSetFramebufferSizeCallback(window_, callback);
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		init_code_ = (int)WINDOWSTATUS_INIT_GLAD_FAILED;
		return;
	}

	init_code_ = (int)WINDOWSTATUS_SUCCESS;
}

Window::Window() 
{
	init_code_ = (int)WINDOWSTATUS_NO_INIT;
	context_ = NULL;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	window_ = glfwCreateWindow(800, 600, "default", NULL, NULL);
	if (window_ == NULL)
	{
		init_code_ = (int)WINDOWSTATUS_CREATRE_WINDOW;
		return;
	}

	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		init_code_ = (int)WINDOWSTATUS_INIT_GLAD_FAILED;
		return;
	}

	glViewport(0, 0, 800,600);

	init_code_ = (int)WINDOWSTATUS_SUCCESS;
}

Window::~Window() 
{
	glfwTerminate();
}

int Window::GetInitializeStatus() const
{
	return init_code_;
}

void Window::GetSize(int* width, int* height)
{
	glfwGetWindowSize(window_, width, height);
}

void Window::Bind(void* context)
{
	context_ = context;
	glfwSetWindowUserPointer(window_, context);
}

void Window::UnBind() 
{
	glfwSetWindowUserPointer(window_, NULL);
	context_ = NULL;
}

void Window::Update(RelRenderFun render_fun) 
{

	double delaTime = 0.0;
	double lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window_)) 
	{
		double currentTime = glfwGetTime();
		delaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		render_fun(this, window_, delaTime,this->context_);

		glfwSwapBuffers(window_);
		glfwPollEvents();
	}

}

void Window::RegisterMousePosition_Callback(GLFWcursorposfun function)
{
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window_,function);
}

void Window::RegisterMouseScroll_Callback(GLFWscrollfun cb)
{
	glfwSetScrollCallback(window_, cb);
}
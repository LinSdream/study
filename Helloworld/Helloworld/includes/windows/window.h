#pragma once
#include<opengl/base.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<functional>

class Window;
typedef  std::function<void(Window* window,GLFWwindow* wwindow, double delaTime,void* obj)> RelRenderFun;
typedef void(*FramebuffSizeCallback)(GLFWwindow* window, int wight, int height);

class Window
{

public:

	Window();
	Window(int wight,int height,char *title, FramebuffSizeCallback callback);
	~Window();

	void RegisterMousePosition_Callback(GLFWcursorposfun cb);
	void RegisterMouseScroll_Callback(GLFWscrollfun cb);

	void Update(RelRenderFun render_fun);
	int GetInitializeStatus() const;
	void GetSize(int* width,int* height);

	void Bind(void* context);
	void UnBind();

	GLFWwindow* GetWindow() const { return window_; }

private:

	GLFWwindow* window_;
	int init_code_;
	void* context_;

};

enum EWindowStatus
{
	WINDOWSTATUS_SUCCESS = 0,
	WINDOWSTATUS_UNKNOWN = 1,
	WINDOWSTATUS_NO_INIT = 2,
	WINDOWSTATUS_INIT_FAILED = 3,
	WINDOWSTATUS_CREATRE_WINDOW = 4,
	WINDOWSTATUS_INIT_GLAD_FAILED = 5,
};
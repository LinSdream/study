#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

typedef void(*RelRenderFun)(GLFWwindow *window,void* obj);
typedef void(*FramebuffSizeCallback)(GLFWwindow* window, int wight, int height);

class Window 
{

public:

	Window();
	Window(int wight,int height,char *title, FramebuffSizeCallback callback);
	~Window();

	void Update(RelRenderFun render_fun);
	int GetInitializeStatus();
	int GetHeight();
	int GetWeight();

	void Bind(void* context);
	void UnBind();

private:

	GLFWwindow* window_;

	int init_code_;
	int height_;
	int weight_;
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
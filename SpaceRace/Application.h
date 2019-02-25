
#ifndef APPLICATION_H
#define APPLICATION_H
//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "timer.h"


class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static HWND windowHWND();
	static GLFWwindow* getGLFWWindow();
	static void GetMousePos(double& x, double& y);
	static bool IsKeyPressed(unsigned short key);
	static bool IsGlfwKeyPressed(int key);


private:

	//Declare a window object
	StopWatch m_timer;
};

#endif
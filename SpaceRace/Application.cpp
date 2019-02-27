

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "Application.h"
#include "Scene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "GameSound.h"
#include "SplashScene.h"
#include "BuildScene.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h); //update opengl the new window size
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursor_enter_callback(GLFWwindow* window, int entered)
{
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << xpos << std::endl;
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::IsGlfwKeyPressed(int glfwKey) {
	if (m_window != nullptr) {
		return glfwGetKey(m_window, glfwKey) == 1;
	}
	return false;
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(1920, 1440, "Test Window", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);
	glfwSetWindowSizeCallback(m_window, resize_callback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(m_window, 1000,700);
	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

HWND Application::windowHWND()  {
	return glfwGetWin32Window(m_window);
}
GLFWwindow* Application::getGLFWWindow() {
	return m_window;
}


void Application::GetMousePos(double& x, double& y) {
	glfwGetCursorPos(m_window, &x, &y);
}

void Application::Run()
{
	MenuFunctionality curFunc;
	curFunc.SetHighlight(new GameObject(MeshBuilder::GenerateRealQuad("HighLight", Color(1, 1, 1), 1, 1)));
	curFunc.AddButton(new GameObject(MeshBuilder::GenerateRealQuad("Button", Color(1, 1, 1), 1, 1)));
	curFunc.AddButton(new GameObject(MeshBuilder::GenerateRealQuad("Button", Color(1, 1, 1), 1, 1)));

	curFunc.InitialiseMenu(-2.0f, 3.0f, 2.0f, 0.5f);

	GameScene* game = new GameScene();

	Scene* scene = new MainMenuScene(curFunc);
	Scene* scene2 = nullptr;
	scene->Init();

	glfwSetCursorPosCallback(m_window, cursor_position_callback);

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.  

		scene->Update(0.01f);
		if (scene2 != nullptr) {
			scene2->Update2(0.01f);
		}
		scene->Render();
		if (scene2 != nullptr) {
			scene2->Render2();
		}

		//Get and organize events, like keyboard and mouse input, window resizing, etc...

		//if (Application::IsKeyPressed(VK_SPACE))
		//{
		//	MainMenu.setPlay(true);
		//	if (Application::IsKeyPressed('W'))
		//	{
		//		MainMenu.setMode(1);
		//		if (Application::IsKeyPressed(VK_SPACE))
		//		{
		//			scene = new GameScene();
		//			scene->Init();
		//		}
		//	}
		//	if (Application::IsKeyPressed('S'))
		//	{
		//		MainMenu.setMode(2);
		//		if (Application::IsKeyPressed(VK_SPACE))
		//		{
		//			scene = new GameScene();
		//			scene->Init();
		//			Scene* scene2 = scene;
		//			scene2->Init2();
		//		}
		//	}
		//}

		if (Application::IsKeyPressed(VK_SPACE) && scene2 == nullptr)
		{
			GameSound::instance()->engine->play2D(GameSound::instance()->MenuSelect);

			if (dynamic_cast<MainMenuScene*>(scene)->menuFunctions.GetChosen() == 0) {
				scene = new GameScene();
				scene->Init();
				scene2 = scene;
				scene2->Init2();
			}
			else 
			{
				scene = new BuildScene();
				scene->Init();
			}
		}

		if (scene2 != nullptr && dynamic_cast<GameScene*>(scene)->returnMainMenu)
		{
			dynamic_cast<GameScene*>(scene)->returnMainMenu = false;
			scene2 = nullptr;
			scene = new MainMenuScene(curFunc);
			scene->Init();
		}

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();

	} //Check if the ESC key had been pressed or if the window had been closed

	scene->Exit();
	delete scene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

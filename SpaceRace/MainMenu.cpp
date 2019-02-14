#include "MainMenu.h"
#include "Scene.h"
#include "Camera3.h"
#include "MatrixStack.h"
#include "MeshBuilder.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include <vector>
#include "shader.hpp"
#include "Utility.h"
#include "Mtx44.h"

#include "Water.h"

#include "GameSound.h"


#include "LoadTGA.h"
#include "Application.h"

MainMenu::MainMenu()
{
	Mesh* MainMenu = MeshBuilder::GenerateQuad("Main Menu", Color(0, 1, 0), 5, 5);


}


MainMenu::~MainMenu()
{
}

void MainMenu::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

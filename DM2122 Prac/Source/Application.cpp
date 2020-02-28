//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include "Application.h"
#include "Camera.h"
#include "SceneText.h"
#include "ShowroomScene.h"
#include "Scene1.h"
#include "TestDriveScene.h"
#include "miniGameTwo.h"
#include "SceneLuckyDraw.h"
#include "SceneRacing.h"
#include "MainMenu.h"
#include "GameMenu.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	//update opengl new window size
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
	
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
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
	m_window = glfwCreateWindow(800, 600, "Test Window", NULL, NULL);
	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

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

	glfwSetWindowSizeCallback(m_window, resize_callback);

}

void Application::Run()
{
	//Main Loop

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		Scene* mainmenu = new SceneText();
		Scene* showroomscene = new ShowroomScene();
		Scene* gamemenu = new GameMenu();
		Scene* luckydraw = new SceneLuckyDraw();
		Scene* matchbp = new miniGameTwo();
		Scene* racing = new SceneRacing();
		Scene* testdrive = new TestDriveScene();
		Scene* scene = testdrive;
		mainmenu->Init();
		showroomscene->Init();
		gamemenu->Init();
		luckydraw->Init();
		matchbp->Init();
		testdrive->Init();

		m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
		while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
		{
			if (mainmenu->Change()) {
				if (IsKeyPressed(VK_RETURN)) {
					Scene* mainmenu = new SceneText();
					mainmenu->Init();
					scene = mainmenu;
				}
				
			}
			if (showroomscene->Change()) {
				if (IsKeyPressed(VK_SHIFT)) {
					Scene* gamemenu = new GameMenu();
					gamemenu->Init();
					scene = gamemenu;
				}
			}
			if (gamemenu->Change()) {
				if (IsKeyPressed('1')) {
					Scene* luckydraw = new SceneLuckyDraw();
					luckydraw->Init();
					scene = luckydraw;
				}
				else if (IsKeyPressed('2')) {
					Scene* matchbp = new miniGameTwo();
					matchbp->Init();
					scene = matchbp;
				}
				if (IsKeyPressed(VK_BACK)) {
					Scene* showroomscene = new ShowroomScene();
					showroomscene->Init();
					scene = showroomscene;
					scene = showroomscene;
				}

			}
			scene->Update(m_timer.getElapsedTime());
			scene->Render();
			//Swap buffers
			glfwSwapBuffers(m_window);
			//Get and organize events, like keyboard and mouse input, window resizing, etc...
			glfwPollEvents();
			m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

		} //Check if the ESC key had been pressed or if the window had been closed
		mainmenu->Exit();
		delete mainmenu;
		showroomscene->Exit();
		delete showroomscene;
		gamemenu->Exit();
		delete gamemenu;
		luckydraw->Exit();
		delete luckydraw;
		matchbp->Exit();
		delete matchbp;
		testdrive->Exit();
		delete testdrive;
	}

}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

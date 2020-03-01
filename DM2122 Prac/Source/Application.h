#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "Scene.h"

enum SCENES {
	MAINMENU,
	SHOWROOM,
	GAMEMENU,
	SHOP,
	GAME1,
	GAME2,
	GAME3,
	SELECTIONMENU,
	TESTDRIVE,
	NUM_SCENE

};
class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static void scenechange(int no);
	static Scene* getScene(int);

private:
	static Scene* scene;
	static Scene* scenes[NUM_SCENE];
	//Declare a window object
	StopWatch m_timer;
};

#endif
#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"
#include "camerafps.h"
#include <iostream>
#include <string>

class GameMenu : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_CURSOR,
		GEO_TEXT,
		GEO_BG,
		NUM_GEOMETRY,
	};

	enum Scene5_UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;
	int cursorx, cursory;
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float sizex, float sizey, float x, float y);
	void CalculateFrameRate();
	void RenderOBJ(Mesh* mesh, TRS& trs, bool end, bool enableLight);

public:
	GameMenu();
	~GameMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual bool Change();
	virtual void Exit();
};

#endif
#ifndef MINIGAMETWO_H
#define MINIGAMETWO_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"
#include "camerafps.h"
#include <iostream>
#include <string>
#include "collision.h"

class miniGameTwo : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_CHAR,
		GEO_TEXT,
		GEO_BG,

		GEO_FRONT,

		GEO_GAMESCREEN,

		GEO_TARGET,
		GEO_BLUEPRINT,
		GEO_SPACING,
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
	TRS chara;

	camerafps camera;

	TRS BLUEPRINT;
	object blueprint;
	Vector3 bluePrintInitialPos;

	TRS SPACING;
	object spacing;

	TRS TARGET;
	object target;

	bool pickBluePrint;
	bool matchBluePrint;
	bool followingTarget;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void CalculateFrameRate();
	void RenderOBJ(Mesh* mesh, TRS& trs, bool end, bool enableLight);
	float movex;
	float movey;
	float movez;
	bool CheckCollision(object& one, object& two);
	void doPickUpCollision();
	void doMatchingCollision();

public:
	miniGameTwo();
	~miniGameTwo();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual bool Change();
	virtual void Exit();
};

#endif
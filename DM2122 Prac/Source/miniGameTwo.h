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
#include "Object.h"
#include "cirObject.h"
#include "rectObject.h"

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
		GEO_BLUEPRINT1,
		GEO_BLUEPRINT2,
		GEO_BLUEPRINT3,
		GEO_SPACING1,
		GEO_SPACING2,
		GEO_SPACING3,
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

	TRS BLUEPRINT1;
	TRS BLUEPRINT2;
	TRS BLUEPRINT3;
	Object* blueprint1;
	Object* blueprint2;
	Object* blueprint3;
	Vector3 bluePrintInitialPos1;
	Vector3 bluePrintInitialPos2;
	Vector3 bluePrintInitialPos3;

	TRS SPACING1;
	TRS SPACING2;
	TRS SPACING3;
	TRS TARGET;
	Object* spacing1;
	Object* spacing2;
	Object* spacing3;
	Object* target;

	bool bp1, bp2, bp3;
	bool space1, space2, space3;
	bool pickBluePrint;
	bool matchBluePrint;
	bool followingTarget;
	int amountOfMoney;
	int numberOfBP;

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
	bool CheckCollision(Object* one, Object* two);
	void doPickUpCollision();
	void doMatchingCollision();

public:
	miniGameTwo();
	~miniGameTwo();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif
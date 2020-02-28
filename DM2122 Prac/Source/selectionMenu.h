#ifndef SELECTIONMENU_H
#define SELECTIONMENU_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"
#include "camerafps.h"
#include <iostream>
#include <string>
#include "collision.h"
#include "carParts.h"

class selectionMenu : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_CHAR,
		GEO_TEXT,

		GEO_MENU,
		GEO_TARGET,

		GEO_CHECKBOX1,
		GEO_CHECKBOX2,
		GEO_CHECKBOX3,
		GEO_CHECKBOX4,
		GEO_CHECKBOX5,
		GEO_CHECKBOX6,

		GEO_CFDESIGN1,
		GEO_CFDESIGN2,
		GEO_CFDESIGN3,
		GEO_CFCOLOUR1,
		GEO_CFCOLOUR2,
		GEO_CFCOLOUR3,
		GEO_CFCOLOUR4,

		GEO_CARPART,
		
		//GEO_RIGHTBUTTON,
		//GEO_LEFTBUTTON,

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
	GEOMETRY_TYPE checkBoxType;

	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;
	TRS chara;

	camerafps camera;

	

	TRS TARGET;
	TRS CHECKBOX1;
	TRS CHECKBOX2;
	TRS CHECKBOX3;
	TRS CHECKBOX4;
	TRS CHECKBOX5;
	TRS CHECKBOX6;
	TRS CARFRAME;
	TRS CFDESIGN1;
	TRS CFDESIGN2;
	TRS CFDESIGN3;
	TRS CFCOLOUR1;
	TRS CFCOLOUR2;
	TRS CFCOLOUR3;
	TRS CFCOLOUR4;

	object target;
	
	bool inCheckBox;
	bool numberBox1;
	bool numberBox2;
	bool numberBox3;
	bool numberBox4;
	bool numberBox5;
	bool numberBox6;
	bool pressedSpace;
	int numberBox;

	bool nextColour;
	bool prevColour;
	bool nextDesign;
	bool prevDesign;
	bool nextButton;
	bool prevButton;
	int cfColourNum;
	int cfDesignNum;
	int wheelDesignNum;
	int windowColourNum;


	void RenderMesh(Mesh* mesh, bool enableLight);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void CalculateFrameRate();
	void RenderOBJ(Mesh* mesh, TRS& trs, bool end, bool enableLight);
	float movex;
	float movey;
	float movez;
	bool CheckCollision(object& one);
	void doCheckBoxCollision();
	

public:
	selectionMenu();
	~selectionMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif
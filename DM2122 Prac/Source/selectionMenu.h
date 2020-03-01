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
#include "Object.h"
#include "cirObject.h"
#include "rectObject.h"
#include"carParts.h"
#include "TestDriveScene.h"

class selectionMenu : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_CHAR,
		GEO_TEXT,
		
		GEO_MENU,
		GEO_TARGET,
		// For the checkboxes
		GEO_CHECKBOX1,
		GEO_CHECKBOX2,
		GEO_CHECKBOX3,
		GEO_CHECKBOX4,
		GEO_CHECKBOX5,
		GEO_CHECKBOX6,
		GEO_CHECKBOX7,
		// For the selection of colours and designs for window,wheel,carframe
		GEO_CFDESIGN1,
		GEO_CFDESIGN2,
		GEO_CFDESIGN3,
		GEO_CFCOLOUR1,
		GEO_CFCOLOUR2,
		GEO_CFCOLOUR3,
		GEO_CFCOLOUR4,
		GEO_WHEELDESIGN1,
		GEO_WHEELDESIGN2,
		GEO_WHEELDESIGN3,
		GEO_WINDOWCOLOUR1,
		GEO_WINDOWCOLOUR2,
		
		GEO_CAR1,
		GEO_CAR2,
		GEO_CAR3,

		GEO_WHEEL1,
		GEO_WHEEL2,
		GEO_WHEEL3,

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

	Camera2 camera;

	TRS TARGET;
	// For the checkboxes
	TRS CHECKBOX1;
	TRS CHECKBOX2;
	TRS MENU;
	TRS CAR1;
	TRS CAR2;
	TRS CAR3;
	TRS WHEEL1;
	TRS WHEEL2;
	TRS WHEEL3;

	Car car;
	Object* target;
	// For the checkboxes
	bool inCheckBox;
	bool numberBox1;
	bool numberBox2;

	bool checkCar1 = false;
	bool checkCar2 = false;
	bool checkCar3 = false;
	bool frameChoice = true;
	bool texChoice = false;
	bool pressedSpace;
	int numberBox;
	// For the selection of colours and designs for window,wheel,carframe

	// To see how many parts are selected
	int numOfParts;


	void RenderMesh(Mesh* mesh, bool enableLight);
	void initCars();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void CalculateFrameRate();
	void RenderOBJ(Mesh* mesh, TRS& trs, bool end, bool enableLight);
	bool CheckCollision(Object* one);
	void doCheckBoxCollision();
	// Movement for target
	float movex;
	float movey;
	float movez;

public:
	selectionMenu();
	~selectionMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif
#ifndef SHOWROOMSCENE_H
#define SHOWROOMSCENE_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"
#include "camerafps.h"
#include "collision.h"

class ShowroomScene : public Scene
{
	enum COLOUR
	{
		RED,
		ORANGE,
		YELLOW,
		GREEN,
		BLUE,
		PURPLE,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,

		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		GEO_CUBE,
		GEO_CHAR,

		GEO_ARCADE,
		GEO_BENCH,
		GEO_BSIGN,
		GEO_CAR1,
		GEO_CAR2,
		GEO_CAR3,
		GEO_CAR4,
		GEO_DOOR,
		GEO_NPC,
		GEO_SCREEN,
		GEO_SCREEN1,
		GEO_SSIGN,
		GEO_SSIGN1,
		GEO_STAGE,
		GEO_STRUCTURESMALL,
		GEO_STRUCTUREBIG,
		GEO_WHEEL1,
		GEO_WHEEL2,
		GEO_WHEEL3,

		GEO_CEILINGLIGHT,
		GEO_SPOTLIGHT,
		GEO_TEXT,

		NUM_GEOMETRY,
	};

	enum Scene5_UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHTENABLED,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_NUMLIGHTS,
		// add these enum for texture
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	/*unsigned m_indexBuffer;*/
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;
	Light light[2];

	camerafps camera;
	
	TRS CUBE;
	TRS PLAYER;
	object cube;
	object player;

	TRS ARCADE;
	TRS BENCH;
	TRS BSIGN;
	TRS CAR1;
	TRS CAR2;
	TRS CAR3;
	TRS CAR4;
	TRS DOOR;
	TRS NPC;
	TRS SCREEN;
	TRS SCREEN1;
	TRS SSIGN;
	TRS SSIGN1;
	TRS STAGE;
	TRS STRUCTURESMALL;
	TRS STRUCTUREBIG;
	TRS WHEEL1;
	TRS WHEEL2;;
	TRS WHEEL3;

	float movex;
	float movey;
	float movez;

	int bouncetime;
	bool switchlights;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void InitObjects();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void CalculateFrameRate();
	void RenderOBJ(Mesh* mesh, TRS& trs, bool end, bool enableLight);

	void InitHeadlights();
	void InitSpotlights();
	void RenderSpotlights();
	void SwitchLightColours();

	bool CheckCollision(object& one, object& two);
	void doCollision();
public:
	ShowroomScene();
	~ShowroomScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif
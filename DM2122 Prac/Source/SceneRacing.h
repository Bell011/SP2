#ifndef _SCENE_RACING_H
#define _SCENE_RACING_H
#include "Scene.h"
#include "g3CarEntity.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"
#include <string.h>
#include <iostream>
#include "collcorners.h"
#include "camerafps.h"
class SceneRacing : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_CHAR,
		GEO_DICE,
		GEO_CAR1,
		GEO_CAR2,
		GEO_CAR3,
		GEO_CAR4,
		GEO_MAP,
		GEO_BARRIER1,
		GEO_BARRIER2,
		GEO_LIGHTSPHERE,
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
		U_LIGHTENABLED,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
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
	Light light[1];

	camerafps camera;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderCars();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderRoad();
	void CalculateFrameRate();

	float carSpeed;
	g3CarEntity* carPlayer = new g3CarEntity;
	corners car;
	g3CarEntity* Car1 = new g3CarEntity;
	g3CarEntity* Car2 = new g3CarEntity;
	g3CarEntity* Car3 = new g3CarEntity;
	
	bool isPlaying;
	float invinTime;
	float bouncetime;
	int lives;
	float timer;

public:
	SceneRacing();
	~SceneRacing();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};



#endif
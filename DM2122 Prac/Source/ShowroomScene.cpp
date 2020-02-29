#include "ShowroomScene.h"
#include "GL\glew.h"

#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

ShowroomScene::ShowroomScene()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

ShowroomScene::~ShowroomScene()
{
}

void ShowroomScene::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader"); 

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 90, 0);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &light[0].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 8);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//indoor_Left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//indoor_Right.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//indoor_Top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//indoor_Bottom.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//indoor_Front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//indoor_Back.tga");

	
	meshList[GEO_CEILINGLIGHT] = MeshBuilder::GenerateQuad("ceilinglight", Color(1.f, 1.f, 1.f), 50, 50);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	InitSpotlights();
	InitHeadlights();
	InitObjects();
	bouncetime = 0.f;
	fps = 0.f;
}
/*=============================================================================*/
/*!
\brief
	Init all the objects

\details
	
	
*/
/*=============================================================================*/
void ShowroomScene::InitObjects() {
	PLAYER.Translate = Vector3(0, 0, 1);
	player = new cirObject(Vector3(PLAYER.Translate.x, PLAYER.Translate.y, PLAYER.Translate.z), 7.f);
	//player = new rectObj(Vector3(7,7,7),Vector3(PLAYER.Translate.x, PLAYER.Translate.y, PLAYER.Translate.z));
	meshList[GEO_ARCADE] = MeshBuilder::GenerateOBJ("Arcade", "OBJ//arcade.obj");
	meshList[GEO_ARCADE]->textureID = LoadTGA("Image//arcade.tga");
	meshList[GEO_ARCADE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_ARCADE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_ARCADE]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_ARCADE]->material.kShininess = 1.f;
	ARCADE.Scale = Vector3(3, 3, 3);
	ARCADE.Translate = Vector3(45, 0, 0);
	ARCADE.RotateY = Vector4(180,0,1,0);
	arcade = new rectObj();
	checkarcade = new rectObj();
	c_arcade.getCoords("OBJ//arcade.obj", c_arcade);
	((rectObj*)arcade)->setSize(c_arcade);
	((rectObj*)arcade)->setManualSize(Vector3(((rectObj*)arcade)->getSize().x * ARCADE.Scale.x, ((rectObj*)arcade)->getSize().y * ARCADE.Scale.y, ((rectObj*)arcade)->getSize().z * ARCADE.Scale.z));
	((rectObj*)checkarcade)->setManualSize(Vector3(((rectObj*)arcade)->getSize().x *1.1, ((rectObj*)arcade)->getSize().y * 1.1, ((rectObj*)arcade)->getSize().z * 1.1));
	arcade->setPos(Vector3(45, 0, 0));
	checkarcade->setPos(Vector3(45, 0, 0));

	meshList[GEO_BENCH] = MeshBuilder::GenerateOBJ("Bench", "OBJ//bench.obj");
	meshList[GEO_BENCH]->textureID = LoadTGA("Image//bench.tga");
	meshList[GEO_BENCH]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BENCH]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BENCH]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BENCH]->material.kShininess = 1.f;
	BENCH.Scale = Vector3(2, 2, 2);
	BENCH.Translate = Vector3(40, -4, -20);
	bench = new rectObj();
	c_bench.getCoords("OBJ//bench.obj", c_bench);
	((rectObj*)bench)->setSize(c_bench);
	((rectObj*)bench)->setManualSize(Vector3(((rectObj*)bench)->getSize().x * BENCH.Scale.x, ((rectObj*)bench)->getSize().y * BENCH.Scale.y, ((rectObj*)bench)->getSize().z * BENCH.Scale.z));
	bench->setPos(Vector3(40, -4, -20));

	meshList[GEO_BSIGN] = MeshBuilder::GenerateOBJ("bigsign", "OBJ//bigsign.obj");
	meshList[GEO_BSIGN]->textureID = LoadTGA("Image//bigsign.tga");
	meshList[GEO_BSIGN]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BSIGN]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BSIGN]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BSIGN]->material.kShininess = 1.f;
	BSIGN.Translate = Vector3(-10,0,-5);
	BSIGN.Scale = Vector3(2,3,2);

	meshList[GEO_DOOR] = MeshBuilder::GenerateCuboid("door", Color(0, 0, 0), 12,8,1);
	meshList[GEO_DOOR]->textureID = LoadTGA("Image//DOOR.tga");
	meshList[GEO_DOOR]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_DOOR]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_DOOR]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_DOOR]->material.kShininess = 1.f;
	DOOR.Translate = Vector3(0, 0, -50);
	DOOR.Scale = Vector3(2, 2, 1);
	door = new rectObj();
	((rectObj*)door)->setManualSize(Vector3(24,8,0.5));
	door->setPos(Vector3(0, 0, -50));

	meshList[GEO_CAR1] = MeshBuilder::GenerateOBJ("car1", "OBJ//car1.obj");
	meshList[GEO_CAR1]->textureID = LoadTGA("Image//car1blue.tga");
	meshList[GEO_CAR1]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CAR1]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR1]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR1]->material.kShininess = 1.f;
	CAR1.Scale = Vector3(3, 3, 3);
	CAR1.Translate = Vector3(42, -1, 30);
	CAR1.RotateY = Vector4(40, 0, 1, 0);
	car1 = new rectObj();
	((rectObj*)car1)->setManualSize(Vector3(3 * CAR1.Scale.x, 3 * CAR1.Scale.y, 3 * CAR1.Scale.z));
	car1->setPos(Vector3(42, -1, 30));
	meshList[GEO_CAR2] = MeshBuilder::GenerateOBJ("car2", "OBJ//car2.obj");
	meshList[GEO_CAR2]->textureID = LoadTGA("Image//car2white2.tga");
	meshList[GEO_CAR2]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CAR2]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR2]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR2]->material.kShininess = 1.f;
	CAR2.Scale = Vector3(3, 3, 3);
	CAR2.Translate = Vector3(0, -1, 30);
	CAR2.RotateY = Vector4(-40, 0, 1, 0);
	car2 = new rectObj();
	((rectObj*)car2)->setManualSize(Vector3(3 * CAR2.Scale.x, 3 * CAR2.Scale.y, 3 * CAR2.Scale.z));
	car2->setPos(Vector3(0, -1, 30));
	meshList[GEO_CAR3] = MeshBuilder::GenerateOBJ("car3", "OBJ//car3.obj");
	meshList[GEO_CAR3]->textureID = LoadTGA("Image//car3red.tga");
	meshList[GEO_CAR3]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CAR3]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR3]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR3]->material.kShininess = 1.f;
	CAR3.Scale = Vector3(3, 3, 3);
	CAR3.Translate = Vector3(-30, 1.5, 20);
	CAR3.RotateY = Vector4(160, 0, 1, 0);
	car3 = new rectObj();
	((rectObj*)car3)->setManualSize(Vector3(3 * CAR3.Scale.x, 3 * CAR3.Scale.y, 3 * CAR3.Scale.z));
	car3->setPos(Vector3(-30, 1.5, 20));
	meshList[GEO_CAR4] = MeshBuilder::GenerateOBJ("car4", "OBJ//car4.obj");
	meshList[GEO_CAR4]->textureID = LoadTGA("Image//car4.tga");
	meshList[GEO_CAR4]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CAR4]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR4]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR4]->material.kShininess = 1.f;
	CAR4.Translate = Vector3(0.5, 2, 0);

	meshList[GEO_NPC] = MeshBuilder::GenerateOBJ("npc", "OBJ//npc.obj");
	meshList[GEO_NPC]->textureID = LoadTGA("Image//npc.tga");
	meshList[GEO_NPC]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_NPC]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_NPC]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_NPC]->material.kShininess = 1.f;
	NPC.Translate = Vector3 (15, -2, -45);
	NPC.RotateY = Vector4(200, 0, 1, 0);
	NPC.Scale = Vector3(2, 2, 2);
	npc = new rectObj();
	checknpc = new rectObj();
	c_npc.getCoords("OBJ//npc.obj", c_npc);
	((rectObj*)npc)->setSize(c_npc);
	((rectObj*)npc)->setManualSize(Vector3(((rectObj*)npc)->getSize().x* NPC.Scale.x, ((rectObj*)npc)->getSize().y* NPC.Scale.y, ((rectObj*)npc)->getSize().z* NPC.Scale.z));
	((rectObj*)checknpc)->setManualSize(Vector3(((rectObj*)npc)->getSize().x* 1.05, ((rectObj*)npc)->getSize().y* 1.05, ((rectObj*)npc)->getSize().z* NPC.Scale.z * 1.05));
	npc->setPos(Vector3(15, -2, -45));
	checknpc->setPos(Vector3(15, -2, -45));

	meshList[GEO_SCREEN] = MeshBuilder::GenerateOBJ("screen", "OBJ//screen.obj");
	meshList[GEO_SCREEN]->textureID = LoadTGA("Image//screen.tga");
	meshList[GEO_SCREEN]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SCREEN]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SCREEN]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SCREEN]->material.kShininess = 1.f;
	SCREEN.Translate = Vector3(-43, 6, -10);
	SCREEN.Scale = Vector3(3, 3, 3);
	screen = new rectObj();
	c_screen.getCoords("OBJ//screen.obj", c_screen);
	((rectObj*)screen)->setSize(c_screen);
	((rectObj*)screen)->setManualSize(Vector3(((rectObj*)screen)->getSize().x* SCREEN.Scale.x, ((rectObj*)screen)->getSize().y* SCREEN.Scale.y, ((rectObj*)screen)->getSize().z* SCREEN.Scale.z));
	screen->setPos(Vector3(-43,6,-10));

	meshList[GEO_SCREEN1] = MeshBuilder::GenerateOBJ("screen1", "OBJ//screen.obj");
	meshList[GEO_SCREEN1]->textureID = LoadTGA("Image//screen2.tga");
	meshList[GEO_SCREEN1]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SCREEN1]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SCREEN1]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SCREEN1]->material.kShininess = 1.f;
	SCREEN1.Translate = Vector3(25, 6, 43);
	SCREEN1.RotateY = Vector4(90, 0, 1, 0);
	SCREEN1.Scale = Vector3(3, 3, 3);
	screen1 = new rectObj();
	((rectObj*)screen1)->setSize(c_screen);
	((rectObj*)screen1)->setManualSize(Vector3(((rectObj*)screen1)->getSize().z* SCREEN1.Scale.z, ((rectObj*)screen1)->getSize().y* SCREEN1.Scale.y, ((rectObj*)screen1)->getSize().x* SCREEN1.Scale.x));
	screen1->setPos(Vector3(25, 6, 43));

	meshList[GEO_SSIGN] = MeshBuilder::GenerateOBJ("smallsign", "OBJ//smallsign.obj");
	meshList[GEO_SSIGN]->textureID = LoadTGA("Image//smallsign.tga");
	meshList[GEO_SSIGN]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SSIGN]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SSIGN]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SSIGN]->material.kShininess = 1.f;
	SSIGN.Translate = Vector3(25, 0, 20);
	SSIGN.RotateY = Vector4(90, 0, 1, 0);
	SSIGN.Scale = Vector3(2, 2, 2);
	ssign = new rectObj();
	((rectObj*)ssign)->setSize(c_ssign);
	((rectObj*)ssign)->setManualSize(Vector3(((rectObj*)ssign)->getSize().z* SSIGN.Scale.z, ((rectObj*)ssign)->getSize().y* SSIGN.Scale.y, ((rectObj*)ssign)->getSize().x* SSIGN.Scale.x));
	ssign->setPos(Vector3(25, 0, 20));

	meshList[GEO_STAGE] = MeshBuilder::GenerateOBJ("stage", "OBJ//stage.obj");
	meshList[GEO_STAGE]->textureID = LoadTGA("Image//stage.tga");
	meshList[GEO_STAGE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_STAGE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_STAGE]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_STAGE]->material.kShininess = 1.f;
	STAGE.Translate = Vector3(-22,-4.5,-15);
	STAGE.Scale = Vector3(3,2.5,3);
	STAGE.RotateY = Vector4(30, 0, 1, 0);
	stage = new rectObj();
	((rectObj*)stage)->setManualSize(Vector3(3.6*STAGE.Scale.x,3.6 * STAGE.Scale.y, 3.6 * STAGE.Scale.z));
	stage->setPos(Vector3(-22, -4.5, -15));

	meshList[GEO_STRUCTURESMALL] = MeshBuilder::GenerateOBJ("structuresmall", "OBJ//structuresmall.obj");
	meshList[GEO_STRUCTURESMALL]->textureID = LoadTGA("Image//structure.tga");
	meshList[GEO_STRUCTURESMALL]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_STRUCTURESMALL]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_STRUCTURESMALL]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_STRUCTURESMALL]->material.kShininess = 1.f;

	meshList[GEO_STRUCTUREBIG] = MeshBuilder::GenerateOBJ("structurebig", "OBJ//structurebig.obj");
	meshList[GEO_STRUCTUREBIG]->textureID = LoadTGA("Image//structure.tga");
	meshList[GEO_STRUCTUREBIG]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_STRUCTUREBIG]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_STRUCTUREBIG]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_STRUCTUREBIG]->material.kShininess = 1.f;
	STRUCTUREBIG.Translate = Vector3(0, -0.5, -15);
	STRUCTUREBIG.Scale = Vector3(1.5, 1.5, 1.5);
	structure = new rectObj();
	((rectObj*)structure)->setManualSize(Vector3(3.6 * STRUCTUREBIG.Scale.x, 6 * STRUCTUREBIG.Scale.y, 3.6 * STRUCTUREBIG.Scale.z));
	structure->setPos(Vector3(0,-0.5,-15));

	meshList[GEO_WHEEL1] = MeshBuilder::GenerateOBJ("wheel1", "OBJ//wheel1.obj");
	meshList[GEO_WHEEL1]->textureID = LoadTGA("Image//wheel1.tga");
	meshList[GEO_WHEEL1]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_WHEEL1]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_WHEEL1]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_WHEEL1]->material.kShininess = 1.f;

	meshList[GEO_WHEEL2] = MeshBuilder::GenerateOBJ("wheel2", "OBJ//wheel2.obj");
	meshList[GEO_WHEEL2]->textureID = LoadTGA("Image//wheel1.tga");
	meshList[GEO_WHEEL2]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_WHEEL2]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_WHEEL2]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_WHEEL2]->material.kShininess = 1.f;

	meshList[GEO_WHEEL3] = MeshBuilder::GenerateOBJ("wheel3", "OBJ//wheel3.obj");
	meshList[GEO_WHEEL3]->textureID = LoadTGA("Image//wheel1.tga");
	meshList[GEO_WHEEL3]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_WHEEL3]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_WHEEL3]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_WHEEL3]->material.kShininess = 1.f;

}

void ShowroomScene::Update(double dt)
{

	//player->setPos(Vector3(camera.position.x,camera.position.y,camera.position.z));
	camera.position = player->getPos();

	Object* objects[11] = {
		arcade,bench,npc,screen,screen1,ssign,car1,car2,car3,stage,structure
	};

	if (Application::IsKeyPressed('A')){
		Vector3 temp = player->getPos();
		Vector3 movement = camera.right;
	//	movement.y = 0;
		movement.Normalize();
		player->setPos(player->getPos() - (movement * dt * 10));
		for (int i = 0; i < 11; i++) {
			if (player->checkCollision(objects[i])) {
				player->setPos(temp);
			}
			camera.position = player->getPos();
		}
	}
	if (Application::IsKeyPressed('D')){
		Vector3 temp = player->getPos();
		Vector3 movement = camera.right;
	//	movement.y = 0;
	//	movement.Normalize();
		player->setPos(player->getPos() + (movement * dt * 10));
		for (int i = 0; i < 11; i++) {
			if (player->checkCollision(objects[i])) {
				player->setPos(temp);
			}
			camera.position = player->getPos();
		}
	}
	if (Application::IsKeyPressed('W')) {
		Vector3 temp = player->getPos();
		Vector3 movement = camera.view;
		movement.y = 0;
		movement.Normalize();
		player->setPos(player->getPos() + (movement * dt * 10));
		for (int i = 0; i < 11; i++) {
			if (player->checkCollision(objects[i])) {
				player->setPos(temp);
			}
			camera.position = player->getPos();
		}
	}
	if (Application::IsKeyPressed('S')) {
		Vector3 temp = player->getPos();
		Vector3 movement = camera.view;
		movement.y = 0;
		movement.Normalize();
		player->setPos(player->getPos() - (movement * dt * 10));
		for (int i = 0; i < 11; i++) {
			if (player->checkCollision(objects[i])) {
				player->setPos(temp);
			}
			camera.position = player->getPos();
		}
	}
	SwitchLightColours();
	//SetCursorPos(camera.setCursorX, camera.setCursorY);
	camera.mouse_callback();
	///camera.Update(dt);
	CalculateFrameRate();
}
/*=============================================================================*/
/*!
\brief
	Alternate method to do TRS/render objects

\param	
	mesh: mesh to render
	TRS: class to set TRS (set in init func)
	end: check whether to pop matrixstack
	enableLight: set Light as true/false
*/
/*=============================================================================*/
void ShowroomScene::RenderOBJ(Mesh* mesh, TRS& trs, bool end, bool enableLight)
{
	modelStack.PushMatrix();
	modelStack.Translate(trs.Translate);
	modelStack.Rotate(trs.RotateX);
	modelStack.Rotate(trs.RotateY);
	modelStack.Rotate(trs.RotateZ);
	modelStack.Scale(trs.Scale);
	RenderMesh(mesh, enableLight);
	if (end) {
		modelStack.PopMatrix();
	}
}
void ShowroomScene::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	// passing the light direction if it is a direction light	
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction 
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}


	RenderSkybox();
	RenderSpotlights();
	RenderHeadlights();

	

	RenderOBJ(meshList[GEO_ARCADE], ARCADE, true, true);
	RenderOBJ(meshList[GEO_BENCH], BENCH, true, true);
	RenderOBJ(meshList[GEO_BSIGN], BSIGN, true, true);

	modelStack.PushMatrix();
	RenderOBJ(meshList[GEO_DOOR], DOOR, false, true);
	modelStack.Translate(-6,5, 0);
	RenderText(meshList[GEO_TEXT], "TEST DRIVING", Color(0, 0, 0));
	modelStack.PopMatrix();

	RenderOBJ(meshList[GEO_NPC], NPC, true, true);
	//NPC.Translate = Vector3(light[0].position.x, light[0].position.y, light[0].position.z);

	RenderOBJ(meshList[GEO_CAR1], CAR1, false, true);
	WHEEL2.Translate = Vector3(0, -0.8, 1.5);
	RenderOBJ(meshList[GEO_WHEEL2], WHEEL2, false, true);
	WHEEL2.Translate = Vector3(0, 0, -3.1);
	RenderOBJ(meshList[GEO_WHEEL2], WHEEL2, false, true);
	WHEEL2.Translate = Vector3(-1.7, 0, 0);
	RenderOBJ(meshList[GEO_WHEEL2], WHEEL2, false, true);
	WHEEL2.Translate = Vector3(0, 0, 3.1);
	RenderOBJ(meshList[GEO_WHEEL2], WHEEL2, true, true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	RenderOBJ(meshList[GEO_CAR2], CAR2, false, true);
	WHEEL3.Translate = Vector3(1.3, -0.8, 1.5);
	RenderOBJ(meshList[GEO_WHEEL3], WHEEL3, false, true);
	WHEEL3.Translate = Vector3(0, 0, -3.1);
	RenderOBJ(meshList[GEO_WHEEL3], WHEEL3, false, true);
	WHEEL3.Translate = Vector3(-2, 0, 0);
	RenderOBJ(meshList[GEO_WHEEL3], WHEEL3, false, true);
	WHEEL3.Translate = Vector3(0, 0, 3.1);
	RenderOBJ(meshList[GEO_WHEEL3], WHEEL3, true, true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	RenderOBJ(meshList[GEO_CAR3], CAR3, false, true);
	WHEEL1.Translate = Vector3(0.5,-1.5,1.75);
	RenderOBJ(meshList[GEO_WHEEL1], WHEEL1, false, true);
	WHEEL1.Translate = Vector3(0, 0, -3.3);
	RenderOBJ(meshList[GEO_WHEEL1], WHEEL1, false, true);
	WHEEL1.Translate = Vector3(-1.7, 0, 0);
	RenderOBJ(meshList[GEO_WHEEL1], WHEEL1, false, true);
	WHEEL1.Translate = Vector3(0, 0, 3.2);
	RenderOBJ(meshList[GEO_WHEEL1], WHEEL1, true, true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	RenderOBJ(meshList[GEO_SCREEN], SCREEN, true, true);
	RenderOBJ(meshList[GEO_SCREEN1], SCREEN1, true, true);
	RenderOBJ(meshList[GEO_SSIGN], SSIGN, true, true);


	RenderOBJ(meshList[GEO_STAGE], STAGE, false, true);
	RenderOBJ(meshList[GEO_CAR4], CAR4, true, true);
	modelStack.PopMatrix();


	RenderOBJ(meshList[GEO_STRUCTUREBIG], STRUCTUREBIG, false, true);
	STRUCTURESMALL.Translate = Vector3(-1, 1, 0);
	RenderOBJ(meshList[GEO_STRUCTURESMALL], STRUCTURESMALL, true, true);
	modelStack.PopMatrix();

	if (player->checkCollision(door)) {
		float currentTime = GetTickCount() * 0.001f;
		if (currentTime - bouncetime > 3.f)
		{
			Application::scenechange(6);
		}
	}
	if (player->checkCollision(checkarcade)) {
		RenderTextOnScreen(meshList[GEO_TEXT], "Press ENTER to Play", Color(0, 0, 0), 3.f, 0, 2);
		if (Application::IsKeyPressed(VK_RETURN)) {
			Application::scenechange(2);
		}
	}
	if (player->checkCollision(checknpc)) {
		RenderTextOnScreen(meshList[GEO_TEXT], "Press ENTER to Buy", Color(0, 0, 0), 3.f, 0, 2);
		if (Application::IsKeyPressed(VK_RETURN)) {
			//Application::scenechange();
		}
	}
	RenderTextOnScreen(meshList[GEO_TEXT], "WASD to move,Mouse to pan", Color(0, 0, 0), 3.f, 0, 1);
	RenderTextOnScreen(meshList[GEO_TEXT], "FPS: " + std::to_string(fps), Color(0, 0, 0), 3.f, 0, 0);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Rotate(90, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_CEILINGLIGHT], false);
	modelStack.PopMatrix();

}

void ShowroomScene::Exit()
{
	// Cleanup here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

void ShowroomScene::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);


	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once in the whole function

	if (mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void ShowroomScene::RenderSkybox()
{
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(-50.f, 45.f, 0.f);
	modelStack.Scale(100.f, 100.f, 100.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(50.f, 45.f, 0.f);
	modelStack.Scale(100.f, 100.f, 100.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 95.f, 0.f);
	modelStack.Scale(100.f, 100.f, 100.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	modelStack.PushMatrix();
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, -5.f, 0.f);
	modelStack.Scale(100.f, 100.f, 100.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	modelStack.PushMatrix();
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_BOTTOM], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 45.f, -50.f);
	modelStack.Scale(100.f, 100.f, 100.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 45.f, 50.f);
	modelStack.Scale(100.f, 100.f, 100.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
}

void ShowroomScene::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);

}

void ShowroomScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void ShowroomScene::CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;
	static float lastTime = 0.0f;
	float currentTime = GetTickCount() * 0.001f;
	++framesPerSecond;
	printf("Current Frames Per Second: %d\n\n", fps);
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
}
void ShowroomScene::InitSpotlights()
{
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	light[1].type = Light::LIGHT_SPOT;
	light[1].position.Set(-12, -3.5, 0);
	light[1].color.Set(0.5f, 0.5f, 0.5f);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(30));
	light[1].cosInner = cos(Math::DegreeToRadian(2));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.65f, -0.2f, 1.f);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &light[1].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	meshList[GEO_SPOTLIGHT] = MeshBuilder::GenerateCuboid("spotlightcover", Color(0.f, 0.5f, 0.5f), 1, 1, 1);
	meshList[GEO_SPOTLIGHT]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SPOTLIGHT]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SPOTLIGHT]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SPOTLIGHT]->material.kShininess = 1.f;
}

void ShowroomScene::RenderSpotlights()
{
	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction 
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	modelStack.PushMatrix();
	modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
	modelStack.Rotate(30, 0, 1, 0);
	RenderMesh(meshList[GEO_SPOTLIGHT], true);
	modelStack.PopMatrix();
}

void ShowroomScene::SwitchLightColours()
{
	int switchColours = ((int)(GetTickCount() * 0.001f)) % 10;

	switch (switchColours % 7)
	{
	case RED:
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		light[1].color.Set(0.5f, 0.f, 0.0f); break;
	case ORANGE:
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		light[1].color.Set(0.5f, 0.5f, 0.0f); break;
	case YELLOW:
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		light[1].color.Set(0.5f, 0.5f, 0.0f); break;
	case GREEN:
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		light[1].color.Set(0.0f, 0.5f, 0.0f); break;
	case BLUE:
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		light[1].color.Set(0.0f, 0.0f, 0.5f); break;
	case PURPLE:
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		light[1].color.Set(0.5f, 0.f, 0.5f); break;
	default:
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		light[1].color.Set(0.5f, 0.5f, 0.5f); break;
	}
}

void ShowroomScene::InitHeadlights()
{
	for (int i = 2; i < 8; i++)
	{
		light[i].type = Light::LIGHT_SPOT;
		light[i].power = 1;
		light[i].kC = 1.f;
		light[i].kL = 0.01f;
		light[i].kQ = 0.001f;
		light[i].cosCutoff = cos(Math::DegreeToRadian(20));
		light[i].cosInner = cos(Math::DegreeToRadian(5));
		light[i].exponent = 3.f;
	}

	light[2].position.Set(32, -0.5, 26.5);
	light[2].spotDirection.Set(0.7f, 0.3f, 1.f);
	light[3].position.Set(36, -0.5, 23);
	light[3].spotDirection.Set(0.6f, 0.3f, 1.f);

	light[4].position.Set(3.5, -1.8, 21);
	light[4].spotDirection.Set(-0.8f, 0.3f, 1.f);
	light[5].position.Set(9.5, -1.8, 26);
	light[5].spotDirection.Set(-0.8f, 0.3f, 1.f);

	light[6].position.Set(-29.5, 0.3, 11.5);
	light[6].spotDirection.Set(-0.45f, 0.3f, 1.f);
	light[7].position.Set(-22.5, 0.3, 14);
	light[7].spotDirection.Set(-0.35f, 0.3f, 1.f);

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");
	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &light[2].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");
	glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
	glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &light[3].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], light[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], light[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);

	m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
	m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
	m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
	m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
	m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
	m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
	m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
	m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
	m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
	m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
	m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");
	glUniform1i(m_parameters[U_LIGHT4_TYPE], light[4].type);
	glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &light[4].color.r);
	glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
	glUniform1f(m_parameters[U_LIGHT4_KC], light[4].kC);
	glUniform1f(m_parameters[U_LIGHT4_KL], light[4].kL);
	glUniform1f(m_parameters[U_LIGHT4_KQ], light[4].kQ);
	glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &light[4].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], light[4].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT4_COSINNER], light[4].cosInner);
	glUniform1f(m_parameters[U_LIGHT4_EXPONENT], light[4].exponent);

	m_parameters[U_LIGHT5_POSITION] = glGetUniformLocation(m_programID, "lights[5].position_cameraspace");
	m_parameters[U_LIGHT5_COLOR] = glGetUniformLocation(m_programID, "lights[5].color");
	m_parameters[U_LIGHT5_POWER] = glGetUniformLocation(m_programID, "lights[5].power");
	m_parameters[U_LIGHT5_KC] = glGetUniformLocation(m_programID, "lights[5].kC");
	m_parameters[U_LIGHT5_KL] = glGetUniformLocation(m_programID, "lights[5].kL");
	m_parameters[U_LIGHT5_KQ] = glGetUniformLocation(m_programID, "lights[5].kQ");
	m_parameters[U_LIGHT5_TYPE] = glGetUniformLocation(m_programID, "lights[5].type");
	m_parameters[U_LIGHT5_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[5].spotDirection");
	m_parameters[U_LIGHT5_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[5].cosCutoff");
	m_parameters[U_LIGHT5_COSINNER] = glGetUniformLocation(m_programID, "lights[5].cosInner");
	m_parameters[U_LIGHT5_EXPONENT] = glGetUniformLocation(m_programID, "lights[5].exponent");
	glUniform1i(m_parameters[U_LIGHT5_TYPE], light[5].type);
	glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &light[5].color.r);
	glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
	glUniform1f(m_parameters[U_LIGHT5_KC], light[5].kC);
	glUniform1f(m_parameters[U_LIGHT5_KL], light[5].kL);
	glUniform1f(m_parameters[U_LIGHT5_KQ], light[5].kQ);
	glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, &light[5].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], light[5].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT5_COSINNER], light[5].cosInner);
	glUniform1f(m_parameters[U_LIGHT5_EXPONENT], light[5].exponent);

	m_parameters[U_LIGHT6_POSITION] = glGetUniformLocation(m_programID, "lights[6].position_cameraspace");
	m_parameters[U_LIGHT6_COLOR] = glGetUniformLocation(m_programID, "lights[6].color");
	m_parameters[U_LIGHT6_POWER] = glGetUniformLocation(m_programID, "lights[6].power");
	m_parameters[U_LIGHT6_KC] = glGetUniformLocation(m_programID, "lights[6].kC");
	m_parameters[U_LIGHT6_KL] = glGetUniformLocation(m_programID, "lights[6].kL");
	m_parameters[U_LIGHT6_KQ] = glGetUniformLocation(m_programID, "lights[6].kQ");
	m_parameters[U_LIGHT6_TYPE] = glGetUniformLocation(m_programID, "lights[6].type");
	m_parameters[U_LIGHT6_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[6].spotDirection");
	m_parameters[U_LIGHT6_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[6].cosCutoff");
	m_parameters[U_LIGHT6_COSINNER] = glGetUniformLocation(m_programID, "lights[6].cosInner");
	m_parameters[U_LIGHT6_EXPONENT] = glGetUniformLocation(m_programID, "lights[6].exponent");
	glUniform1i(m_parameters[U_LIGHT6_TYPE], light[6].type);
	glUniform3fv(m_parameters[U_LIGHT6_COLOR], 1, &light[6].color.r);
	glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
	glUniform1f(m_parameters[U_LIGHT6_KC], light[6].kC);
	glUniform1f(m_parameters[U_LIGHT6_KL], light[6].kL);
	glUniform1f(m_parameters[U_LIGHT6_KQ], light[6].kQ);
	glUniform3fv(m_parameters[U_LIGHT6_SPOTDIRECTION], 1, &light[6].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT6_COSCUTOFF], light[6].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT6_COSINNER], light[6].cosInner);
	glUniform1f(m_parameters[U_LIGHT6_EXPONENT], light[6].exponent);

	m_parameters[U_LIGHT7_POSITION] = glGetUniformLocation(m_programID, "lights[7].position_cameraspace");
	m_parameters[U_LIGHT7_COLOR] = glGetUniformLocation(m_programID, "lights[7].color");
	m_parameters[U_LIGHT7_POWER] = glGetUniformLocation(m_programID, "lights[7].power");
	m_parameters[U_LIGHT7_KC] = glGetUniformLocation(m_programID, "lights[7].kC");
	m_parameters[U_LIGHT7_KL] = glGetUniformLocation(m_programID, "lights[7].kL");
	m_parameters[U_LIGHT7_KQ] = glGetUniformLocation(m_programID, "lights[7].kQ");
	m_parameters[U_LIGHT7_TYPE] = glGetUniformLocation(m_programID, "lights[7].type");
	m_parameters[U_LIGHT7_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[7].spotDirection");
	m_parameters[U_LIGHT7_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[7].cosCutoff");
	m_parameters[U_LIGHT7_COSINNER] = glGetUniformLocation(m_programID, "lights[7].cosInner");
	m_parameters[U_LIGHT7_EXPONENT] = glGetUniformLocation(m_programID, "lights[7].exponent");
	glUniform1i(m_parameters[U_LIGHT7_TYPE], light[7].type);
	glUniform3fv(m_parameters[U_LIGHT7_COLOR], 1, &light[7].color.r);
	glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
	glUniform1f(m_parameters[U_LIGHT7_KC], light[7].kC);
	glUniform1f(m_parameters[U_LIGHT7_KL], light[7].kL);
	glUniform1f(m_parameters[U_LIGHT7_KQ], light[7].kQ);
	glUniform3fv(m_parameters[U_LIGHT7_SPOTDIRECTION], 1, &light[7].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT7_COSCUTOFF], light[7].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT7_COSINNER], light[7].cosInner);
	glUniform1f(m_parameters[U_LIGHT7_EXPONENT], light[7].exponent);

	meshList[GEO_HEADLIGHTS] = MeshBuilder::GenerateOBJ("headlights1", "OBJ//headlights1.obj");
	meshList[GEO_HEADLIGHTS]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_HEADLIGHTS]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_HEADLIGHTS]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_HEADLIGHTS]->material.kShininess = 1.f;
}

void ShowroomScene::RenderHeadlights()
{
	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction
	else if (light[2].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[2].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[3].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[3].position.x, light[3].position.y, light[3].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction
	else if (light[3].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[3].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[4].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[4].position.x, light[4].position.y, light[4].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction
	else if (light[4].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[4].position;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[4].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[4].position;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[5].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[5].position.x, light[5].position.y, light[5].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction
	else if (light[5].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[5].position;
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[5].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[5].position;
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[6].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[6].position.x, light[6].position.y, light[6].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction
	else if (light[6].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[6].position;
		glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[6].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT6_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[6].position;
		glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (light[7].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[7].position.x, light[7].position.y, light[7].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction
	else if (light[7].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[7].position;
		glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[7].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT7_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[7].position;
		glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightPosition_cameraspace.x);
	}

	for (int i = 2; i < 8; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(light[i].position.x, light[i].position.y, light[i].position.z);

		if (i == 2 || i == 3)
		{

			modelStack.Rotate(40, 0, 1, 0);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(3, 3, 3);
		}

		if (i == 4 || i == 5)
		{
			modelStack.Rotate(180, 1, 0, 0);
			modelStack.Scale(4, 4, 4);
			modelStack.Rotate(40, 0, 1, 0);
		}

		if (i == 6 || i == 7)
		{
			modelStack.Rotate(70, 0, 1, 0);
			modelStack.Rotate(-40, 0, 0, 1);
			modelStack.Scale(4.5, 4.5, 4.5);
		}

		RenderMesh(meshList[GEO_HEADLIGHTS], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(-19.3, 0.5, -5.7);
	modelStack.Rotate(30, 0, 1, 0);
	modelStack.Scale(2.5, 2.5, 2.5);
	RenderMesh(meshList[GEO_HEADLIGHTS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-14, 0.5, -8.9);
	modelStack.Rotate(30, 0, 1, 0);
	modelStack.Scale(2.5, 2.5, 2.5);
	RenderMesh(meshList[GEO_HEADLIGHTS], true);
	modelStack.PopMatrix();
}
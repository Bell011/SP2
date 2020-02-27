#include "selectionMenu.h"
#include "carParts.h"
#include "GL\glew.h"

#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <string>

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

selectionMenu::selectionMenu()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

selectionMenu::~selectionMenu()
{

}

void selectionMenu::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	camera.Init(Vector3(0, -10, 10), Vector3(0, -10, 9), Vector3(0, 1, 0));
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	//glEnable(GL_DEPTH_TEST);
	glUseProgram(m_programID);

	meshList[GEO_MENU] = MeshBuilder::GenerateQuad("menu", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_MENU]->textureID = LoadTGA("Image//menu_texture.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	
	meshList[GEO_TARGET] = MeshBuilder::GenerateOBJ("target", "OBJ//target.obj");
	meshList[GEO_TARGET]->textureID = LoadTGA("Image//targetTexture.tga");
	meshList[GEO_TARGET]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_TARGET]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TARGET]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TARGET]->material.kShininess = 1.f;

	movex = 0; movey = -10; movez = 0;
	
	meshList[GEO_CHECKBOX1] = MeshBuilder::GenerateQuad("checkBox1", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX1.Translate = Vector3(1.5, -8.4,0);
	checkbox1.pos = Vector3(CHECKBOX1.Translate.x, CHECKBOX1.Translate.y, CHECKBOX1.Translate.z);
	meshList[GEO_CHECKBOX1]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CHECKBOX1]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX1]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX1]->material.kShininess = 1.f;

	meshList[GEO_CHECKBOX2] = MeshBuilder::GenerateQuad("checkBox2", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX2.Translate = Vector3(1.5, -9.1, 0);
	checkbox2.pos = Vector3(CHECKBOX2.Translate.x, CHECKBOX2.Translate.y, CHECKBOX2.Translate.z);
	meshList[GEO_CHECKBOX2]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CHECKBOX2]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX2]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX2]->material.kShininess = 1.f;

	meshList[GEO_CHECKBOX3] = MeshBuilder::GenerateQuad("checkBox3", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX3.Translate = Vector3(1.5, -9.8, 0);
	checkbox3.pos = Vector3(CHECKBOX3.Translate.x, CHECKBOX3.Translate.y, CHECKBOX3.Translate.z);
	meshList[GEO_CHECKBOX3]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CHECKBOX3]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX3]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX3]->material.kShininess = 1.f;

	meshList[GEO_CHECKBOX4] = MeshBuilder::GenerateQuad("checkBox4", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX4.Translate = Vector3(1.5, -10.5, 0);
	checkbox4.pos = Vector3(CHECKBOX4.Translate.x, CHECKBOX4.Translate.y, CHECKBOX4.Translate.z);
	meshList[GEO_CHECKBOX4]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CHECKBOX4]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX4]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX4]->material.kShininess = 1.f;

	meshList[GEO_CHECKBOX5] = MeshBuilder::GenerateQuad("checkBox5", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX5.Translate = Vector3(1.5, -11.2, 0);
	checkbox5.pos = Vector3(CHECKBOX5.Translate.x, CHECKBOX5.Translate.y, CHECKBOX5.Translate.z);
	meshList[GEO_CHECKBOX5]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CHECKBOX5]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX5]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX5]->material.kShininess = 1.f;

	meshList[GEO_CHECKBOX6] = MeshBuilder::GenerateQuad("checkBox6", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX6.Translate = Vector3(1.5, -11.9, 0);
	checkbox6.pos = Vector3(CHECKBOX6.Translate.x, CHECKBOX6.Translate.y, CHECKBOX6.Translate.z);
	meshList[GEO_CHECKBOX6]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CHECKBOX6]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX6]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX6]->material.kShininess = 1.f;

	meshList[GEO_RIGHTBUTTON] = MeshBuilder::GenerateQuad("rightButton", Color(0, 0, 0), 0.4f, 0.4f);
	RIGHTBUTTON.Translate = Vector3(4.7, -9.4, 0);
	rightbutton.pos = Vector3(RIGHTBUTTON.Translate.x, RIGHTBUTTON.Translate.y, RIGHTBUTTON.Translate.z);
	meshList[GEO_RIGHTBUTTON]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_RIGHTBUTTON]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_RIGHTBUTTON]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_RIGHTBUTTON]->material.kShininess = 1.f;
	
	meshList[GEO_LEFTBUTTON] = MeshBuilder::GenerateQuad("leftButton", Color(0, 0, 0), 0.4f, 0.4f);
	LEFTBUTTON.Translate = Vector3(2.7, -9.4, 0);
	leftbutton.pos = Vector3(LEFTBUTTON.Translate.x, LEFTBUTTON.Translate.y, LEFTBUTTON.Translate.z);
	meshList[GEO_LEFTBUTTON]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_LEFTBUTTON]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_LEFTBUTTON]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_LEFTBUTTON]->material.kShininess = 1.f;

	meshList[GEO_NUMBERBUTTON] = MeshBuilder::GenerateQuad("numberButton", Color(0, 0, 0), 0.4f, 0.4f);
	NUMBERBUTTON.Translate = Vector3(3.7, -9.4, 0);
	numberbutton.pos = Vector3(NUMBERBUTTON.Translate.x, NUMBERBUTTON.Translate.y, NUMBERBUTTON.Translate.z);
	meshList[GEO_NUMBERBUTTON]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_NUMBERBUTTON]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_NUMBERBUTTON]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_NUMBERBUTTON]->material.kShininess = 1.f;

	inCheckBox = false;
	pressedSpace = false;
	numberBox = 0;
	numberBox1 = numberBox2 = numberBox3 = numberBox4 = numberBox5 = numberBox6 = false;
	cfColourNum = cfDesignNum = 1;
}

void selectionMenu::Update(double dt)
{
	if (Application::IsKeyPressed(0x31))
	{
		glDisable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x32))
	{
		glEnable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x33))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Application::IsKeyPressed(0x34))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	CalculateFrameRate();

	if (Application::IsKeyPressed(VK_LEFT))
	{
		movex -= 5 * dt;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		movex += 5 * dt;
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		movey += 5 * dt;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		movey -= 5 * dt;
	}
	std::cout << target.pos.x << " " << target.pos.y << std::endl;
	if (Application::IsKeyPressed(VK_SPACE))
	{
		static float lastTime = 0.0f;
		float currentTime = GetTickCount() * 0.001f;

		if (currentTime - lastTime > 0.5f)
		{
			lastTime = currentTime;

			if (!pressedSpace) // when player try to check a box
			{
				doCheckBoxCollision(); // check collision
				pressedSpace = true;
			}
		}	
	}
}

void selectionMenu::RenderOBJ(Mesh* mesh, TRS& trs, bool end, bool enableLight)
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

void selectionMenu::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	//Render in order from Background to Foreground
	RenderMeshOnScreen(meshList[GEO_MENU], 40, 60, 0.5, 0.5);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "INVENTORY", Color(0, 0, 0), 4, 5, 14);
	RenderTextOnScreen(meshList[GEO_TEXT], "CUSTOMISE YOUR CAR TO", Color(0, 0, 0), 2.5, 5, 21);
	RenderTextOnScreen(meshList[GEO_TEXT], "TEST DRIVE", Color(0, 0, 0), 2.5, 10, 20);

	// Parts that do not need any customization
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF ENGINE PARTS: 1", Color(0, 0, 0), 1.7, 0, 24); // need 1 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF HEADLIGHT PARTS: 2", Color(0, 0, 0), 1.7, 0, 21); // need 2 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF BRAKELIGHT PARTS: 2", Color(0, 0, 0), 1.7, 0, 18); // need 2 to be counted

	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF WINDOW PARTS: 6", Color(0, 0, 0), 1.7, 0, 15); // need 6 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF WHEEL PARTS: 4", Color(0, 0, 0), 1.7, 0, 12); // need 4 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF CARFRAME PARTS: 1", Color(0, 0, 0), 1.7, 0, 9); // need 1 to be counted


	RenderTextOnScreen(meshList[GEO_TEXT], " 0/6 CAR PARTS SELECTED", Color(0, 0, 0), 2.5, 3, 2);
	RenderTextOnScreen(meshList[GEO_TEXT], " PRESS 'SPACE' TO SELECT", Color(0, 0, 0), 3, 1, 0);

	TARGET.Translate = Vector3(movex, movey, movez);
	target.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z);
	RenderOBJ(meshList[GEO_TARGET], TARGET, true, true);

	if (pressedSpace) // player try to check a box
	{
		if (inCheckBox) // collided
		{
			// no fill->fill
			if (numberBox == 1)
			{
				if (!numberBox1)
				{
					meshList[GEO_CHECKBOX1]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX1], CHECKBOX1, true, true);
					numberBox1 = true;
				}
				else if (numberBox1)
				{
					meshList[GEO_CHECKBOX1] = MeshBuilder::GenerateQuad("checkBox1", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox1 = false;
				}
			}
			if (numberBox == 2)
			{
				if (!numberBox2)
				{
					meshList[GEO_CHECKBOX2]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX2], CHECKBOX2, true, true);
					numberBox2 = true;
				}
				else if (numberBox2)
				{
					meshList[GEO_CHECKBOX2] = MeshBuilder::GenerateQuad("checkBox2", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox2 = false;
				}
			}
			if (numberBox == 3)
			{
				if (!numberBox3)
				{
					meshList[GEO_CHECKBOX3]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX3], CHECKBOX3, true, true);
					numberBox3 = true;
				}
				else if (numberBox3)
				{
					meshList[GEO_CHECKBOX3] = MeshBuilder::GenerateQuad("checkBox3", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox3 = false;
				}				}
			if (numberBox == 4)
			{
				if (!numberBox4)
				{
					meshList[GEO_CHECKBOX4]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX4], CHECKBOX4, true, true);
					numberBox4 = true;
				}
				else if (numberBox4)
				{
					meshList[GEO_CHECKBOX4] = MeshBuilder::GenerateQuad("checkBox4", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox4 = false;
				}
			}
			if (numberBox == 5)
			{
				if (!numberBox5)
				{
					meshList[GEO_CHECKBOX5]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX5], CHECKBOX5, true, true);
					numberBox5 = true;
				}
				else if (numberBox5)
				{
					meshList[GEO_CHECKBOX5] = MeshBuilder::GenerateQuad("checkBox5", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox5 = false;
				}
			}
			if (numberBox == 6) // Does not occur every frame when the last box is pressed
			{
				if (!numberBox6) // Does not occur every frame
				{
					meshList[GEO_CHECKBOX6]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX6], CHECKBOX6, true, true);
					numberBox6 = true;
				}
				else if (numberBox6)
				{
					meshList[GEO_CHECKBOX6] = MeshBuilder::GenerateQuad("checkBox6", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox6 = false;
				}
			}
			inCheckBox = false;
		}
		else if (!inCheckBox) // no collision, remains the same
		{
			RenderOBJ(meshList[GEO_CHECKBOX1], CHECKBOX1, true, true);
			RenderOBJ(meshList[GEO_CHECKBOX2], CHECKBOX2, true, true);
			RenderOBJ(meshList[GEO_CHECKBOX3], CHECKBOX3, true, true);
			RenderOBJ(meshList[GEO_CHECKBOX4], CHECKBOX4, true, true);
			RenderOBJ(meshList[GEO_CHECKBOX5], CHECKBOX5, true, true);
			RenderOBJ(meshList[GEO_CHECKBOX6], CHECKBOX6, true, true);
		}
		pressedSpace = false;
	}
	else if (!pressedSpace)
	{
		RenderOBJ(meshList[GEO_CHECKBOX1], CHECKBOX1, true, true);
		RenderOBJ(meshList[GEO_CHECKBOX2], CHECKBOX2, true, true);
		RenderOBJ(meshList[GEO_CHECKBOX3], CHECKBOX3, true, true);
		RenderOBJ(meshList[GEO_CHECKBOX4], CHECKBOX4, true, true);
		RenderOBJ(meshList[GEO_CHECKBOX5], CHECKBOX5, true, true);
		RenderOBJ(meshList[GEO_CHECKBOX6], CHECKBOX6, true, true);
	}
	// For the choosing of designs and colours of the car parts 
	//if (numberBox == 4 && numberBox4)
	//{
	//	//Window
	//	RenderTextOnScreen(meshList[GEO_TEXT], " CHOOSE YOUR", Color(0, 0, 0), 2, 27, 20);
	//	RenderTextOnScreen(meshList[GEO_TEXT], "WINDOW:", Color(0, 0, 0), 2, 29, 19);

	//	RenderTextOnScreen(meshList[GEO_TEXT], "<", Color(0, 0, 0), 4, 14.5, 8.1);
	//	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(numberWindows), Color(0, 0, 0), 4, 16.5, 8.1);
	//	RenderTextOnScreen(meshList[GEO_TEXT], ">", Color(0, 0, 0), 4, 18.5, 8.1);

	//	if (rightArrow)
	//	{
	//		if (numberWindows < 3)
	//		{
	//			numberWindows++;
	//		}
	//		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(numberWindows), Color(0, 0, 0), 4, 16.5, 8.1);
	//		rightArrow = false;
	//	}
	//	if (leftArrow)
	//	{
	//		if (numberWindows > 1)
	//		{
	//			numberWindows--;
	//		}
	//		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(numberWindows), Color(0, 0, 0), 4, 16.5, 8.1);
	//		leftArrow = false;
	//	}
	//}
	//else if (numberBox == 5 && numberBox5) 
	//{
	//	// Wheels
	//	RenderTextOnScreen(meshList[GEO_TEXT], " CHOOSE YOUR", Color(0, 0, 0), 2, 27, 20); 
	//	RenderTextOnScreen(meshList[GEO_TEXT], "WHEELS:", Color(0, 0, 0), 2, 29, 19); 

	//	RenderTextOnScreen(meshList[GEO_TEXT], "<", Color(0, 0, 0), 4, 14.5, 8.1);
	//	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(numberWheels), Color(0, 0, 0), 4, 16.5, 8.1);
	//	RenderTextOnScreen(meshList[GEO_TEXT], ">", Color(0, 0, 0), 4, 18.5, 8.1);

	//	if (rightArrow)
	//	{
	//		if (numberWheels < 3)
	//		{
	//			numberWheels++;
	//		}
	//		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(numberWheels), Color(0, 0, 0), 4, 16.5, 8.1);
	//		rightArrow = false;
	//	}
	//	if (leftArrow)
	//	{
	//		if (numberWheels > 1)
	//		{
	//			numberWheels--;
	//		}
	//		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(numberWheels), Color(0, 0, 0), 4, 16.5, 8.1);
	//		leftArrow = false;
	//	}
	//}
	if (numberBox == 6 && numberBox6) 
	{
		// Carframe
		// Choose colours
		RenderTextOnScreen(meshList[GEO_TEXT], " CHOOSE YOUR", Color(0, 0, 0), 2, 27, 22.5); 
		RenderTextOnScreen(meshList[GEO_TEXT], "COLOUR:", Color(0, 0, 0), 2, 30, 21.5); 
		RenderTextOnScreen(meshList[GEO_TEXT], "<", Color(0, 0, 0), 3, 20.5, 13);
		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(cfColourNum), Color(0, 0, 0), 3, 22, 13);
		RenderTextOnScreen(meshList[GEO_TEXT], ">", Color(0, 0, 0), 3, 23.5, 13);
		RenderTextOnScreen(meshList[GEO_TEXT], "NEXT", Color(0, 0, 0), 1.5, 49, 26.5);
		RenderTextOnScreen(meshList[GEO_TEXT], "PREV", Color(0, 0, 0), 1.5, 36, 26.5);

		// Choose design
		RenderTextOnScreen(meshList[GEO_TEXT], " CHOOSE YOUR", Color(0, 0, 0), 2, 27, 12.5);
		RenderTextOnScreen(meshList[GEO_TEXT], "DESIGN:", Color(0, 0, 0), 2, 30, 11.5);
		RenderTextOnScreen(meshList[GEO_TEXT], "<", Color(0, 0, 0), 3, 20.5, 6.5);
		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(cfDesignNum), Color(0, 0, 0), 3, 22, 6.5);
		RenderTextOnScreen(meshList[GEO_TEXT], ">", Color(0, 0, 0), 3, 23.5, 6.5);
		RenderTextOnScreen(meshList[GEO_TEXT], "NEXT", Color(0, 0, 0), 1.5, 49, 13.5);
		RenderTextOnScreen(meshList[GEO_TEXT], "PREV", Color(0, 0, 0), 1.5, 36, 13.5);

		if (nextColour)
		{
			if (cfColourNum < 4)
			{
				cfColourNum++;
			}
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(cfColourNum), Color(0, 0, 0), 3, 22, 13);
			nextColour = false;
		}
		if (prevColour)
		{
			if (cfColourNum > 1)
			{
				cfColourNum--;
			}
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(cfColourNum), Color(0, 0, 0), 3, 22, 13);
			prevColour = false;
		}
		if (nextDesign)
		{
			if (cfDesignNum < 3)
			{
				cfDesignNum++;
			}
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(cfDesignNum), Color(0, 0, 0), 3, 22, 6.5);
			nextDesign = false;
		}
		if (prevDesign)
		{
			if (cfDesignNum > 1)
			{
				cfDesignNum--;
			}
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(cfDesignNum), Color(0, 0, 0), 3, 22, 6.5);
			prevDesign = false;
		}
	}
	
	
	/*carParts* car;
	car->getCarWheel();
	std::cout << car;*/
}

/******************************************************************************/
/*!
\brief
	Rendering meshes on 2D screen

\param	mesh
	mesh to render
\param	sizex
	Factor to scale along x-axis
\param	sizey
	Factor to scale along x-axis
\param	x
	Offset along x-axis
\param	y
	Offset along y-axis
*/
/******************************************************************************/
void selectionMenu::RenderMeshOnScreen(Mesh* mesh, float sizex, float sizey, float x, float y) {
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 40, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(sizex, sizey, 1);
	modelStack.Translate(x, y, 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	RenderMesh(mesh, false);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
}

void selectionMenu::Exit()
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

void selectionMenu::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

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

void selectionMenu::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
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

void selectionMenu::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void selectionMenu::CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;
	static int fps;
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

bool selectionMenu::CheckCollision(object& one, object& two)
{
	if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -8.595 && one.pos.y <= -8.168)) // Checkbox1
	{
		numberBox = 1;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -9.268 && one.pos.y <= -8.928)) // Checkbox2
	{
		numberBox = 2;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -10.033 && one.pos.y <= -9.609)) // Checkbox3
	{
		numberBox = 3;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -10.670 && one.pos.y <= -10.328)) // Checkbox4
	{
		numberBox = 4;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -11.431 && one.pos.y <= -11.012)) // Checkbox5
	{
		numberBox = 5;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -12.111 && one.pos.y <= -11.685)) // Checkbox6
	{
		numberBox = 6;
		inCheckBox = true;
		return true;
	}

	// Check whether collided with prev and next buttons for carframe
	else if (numberBox6 && (one.pos.x >= 1.882 && one.pos.x <= 2.733) && (one.pos.y >= -8.702 && one.pos.y <= -8.447)) 
	{
		prevColour = true;
	}
	else if (numberBox6 && (one.pos.x >= 4.607 && one.pos.x <= 5.374) && (one.pos.y >= -8.702 && one.pos.y <= -8.447))
	{
		nextColour = true;
	}
	else if (numberBox6 && (one.pos.x >= 1.882 && one.pos.x <= 2.733) && (one.pos.y >= -11.347 && one.pos.y <= -11.091)) 
	{
		prevDesign = true;
	}
	else if (numberBox6 && (one.pos.x >= 4.607 && one.pos.x <= 5.374) && (one.pos.y >= -11.347 && one.pos.y <= -11.091))
	{
		nextDesign = true;
	}

	return false;
}

void selectionMenu::doCheckBoxCollision()
{
	// Check whether target has collision with any checkboxes
	CheckCollision(target, checkbox1);
}
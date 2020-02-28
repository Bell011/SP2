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
	movex = 0; movey = -10; movez = 0;
	
	meshList[GEO_CHECKBOX1] = MeshBuilder::GenerateQuad("checkBox1", Color(0, 0, 0), 0.4f, 0.4f); // Suspension
	CHECKBOX1.Translate = Vector3(1.5, -7.7,0);

	meshList[GEO_CHECKBOX2] = MeshBuilder::GenerateQuad("checkBox2", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX2.Translate = Vector3(1.5, -8.4, 0);

	meshList[GEO_CHECKBOX3] = MeshBuilder::GenerateQuad("checkBox3", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX3.Translate = Vector3(1.5, -9.1, 0);

	meshList[GEO_CHECKBOX4] = MeshBuilder::GenerateQuad("checkBox4", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX4.Translate = Vector3(1.5, -9.8, 0);

	meshList[GEO_CHECKBOX5] = MeshBuilder::GenerateQuad("checkBox5", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX5.Translate = Vector3(1.5, -10.5, 0);

	meshList[GEO_CHECKBOX6] = MeshBuilder::GenerateQuad("checkBox6", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX6.Translate = Vector3(1.5, -11.2, 0);

	meshList[GEO_CHECKBOX7] = MeshBuilder::GenerateQuad("checkBox7", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX7.Translate = Vector3(1.5, -11.9, 0);

	meshList[GEO_CFDESIGN1] = MeshBuilder::GenerateQuad("cfdesign1", Color(0, 0, 0),1.f, 1.f);
	meshList[GEO_CFDESIGN1]->textureID = LoadTGA("Image//cfDesignNum1.tga");
	CFDESIGN1.Translate = Vector3(3.7, -12.3, 0);
	CFDESIGN1.Scale = Vector3(2, 1.3, 1);

	meshList[GEO_CFDESIGN2] = MeshBuilder::GenerateQuad("cfdesign2", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_CFDESIGN2]->textureID = LoadTGA("Image//cfDesignNum2.tga");
	CFDESIGN2.Translate = Vector3(3.7, -12.3, 0);
	CFDESIGN2.Scale = Vector3(2, 1.3, 1);

	meshList[GEO_CFDESIGN3] = MeshBuilder::GenerateQuad("cfdesign3", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_CFDESIGN3]->textureID = LoadTGA("Image//cfDesignNum3.tga");
	CFDESIGN3.Translate = Vector3(3.7, -12.3, 0);
	CFDESIGN3.Scale = Vector3(2, 1.3, 1);

	meshList[GEO_CFCOLOUR1] = MeshBuilder::GenerateQuad("cfcolour1", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_CFCOLOUR1]->textureID = LoadTGA("Image//cfColourNum1.tga");
	CFCOLOUR1.Translate = Vector3(3.7, -9.5, 0);
	CFCOLOUR1.Scale = Vector3(2, 1.3, 1);

	meshList[GEO_CFCOLOUR2] = MeshBuilder::GenerateQuad("cfcolour2", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_CFCOLOUR2]->textureID = LoadTGA("Image//cfColourNum2.tga");
	CFCOLOUR2.Translate = Vector3(3.7, -9.5, 0);
	CFCOLOUR2.Scale = Vector3(2, 1.3, 1);

	meshList[GEO_CFCOLOUR3] = MeshBuilder::GenerateQuad("cfcolour3", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_CFCOLOUR3]->textureID = LoadTGA("Image//cfColourNum3.tga");
	CFCOLOUR3.Translate = Vector3(3.7, -9.5, 0);
	CFCOLOUR3.Scale = Vector3(2, 1.3, 1);

	meshList[GEO_CFCOLOUR4] = MeshBuilder::GenerateQuad("cfcolour4", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_CFCOLOUR4]->textureID = LoadTGA("Image//cfColourNum4.tga");
	CFCOLOUR4.Translate = Vector3(3.7, -9.5, 0);
	CFCOLOUR4.Scale = Vector3(2, 1.3, 1);
	
	meshList[GEO_WHEELDESIGN1] = MeshBuilder::GenerateQuad("wheeldesign1", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_WHEELDESIGN1]->textureID = LoadTGA("Image//wheelDesignNum1.tga");
	WHEELDESIGN1.Translate = Vector3(3.7, -10.5, 0);
	WHEELDESIGN1.Scale = Vector3(2, 1.3, 1);

	meshList[GEO_WHEELDESIGN2] = MeshBuilder::GenerateQuad("wheeldesign2", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_WHEELDESIGN2]->textureID = LoadTGA("Image//wheelDesignNum2.tga");
	WHEELDESIGN2.Translate = Vector3(3.7, -10.5, 0);
	WHEELDESIGN2.Scale = Vector3(2, 1.3, 1);

	meshList[GEO_WHEELDESIGN3] = MeshBuilder::GenerateQuad("wheeldesign3", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_WHEELDESIGN3]->textureID = LoadTGA("Image//wheelDesignNum3.tga");
	WHEELDESIGN3.Translate = Vector3(3.7, -10.5, 0);
	WHEELDESIGN3.Scale = Vector3(2, 1.3, 1);

	meshList[GEO_WINDOWCOLOUR1] = MeshBuilder::GenerateQuad("windowcolour1", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_WINDOWCOLOUR1]->textureID = LoadTGA("Image//windowColourNum1.tga");
	WINDOWCOLOUR1.Translate = Vector3(3.7, -10.5, 0);
	WINDOWCOLOUR1.Scale = Vector3(2, 1.3, 1);

	meshList[GEO_WINDOWCOLOUR2] = MeshBuilder::GenerateQuad("windowcolour2", Color(0, 0, 0), 1.f, 1.f);
	meshList[GEO_WINDOWCOLOUR2]->textureID = LoadTGA("Image//windowColourNum2.tga");
	WINDOWCOLOUR2.Translate = Vector3(3.7, -10.5, 0);
	WINDOWCOLOUR2.Scale = Vector3(2, 1.3, 1);

	// For the checkboxes
	inCheckBox = false;
	numberBox1 = numberBox2 = numberBox3 = numberBox4 = numberBox5 = numberBox6 = false;
	pressedSpace = false;
	numberBox = 0;
	// For the selection of colours and designs
	cfColourNum = cfDesignNum = wheelDesignNum = windowColourNum = 0;
	nextButton = prevButton = false;
	nextColour = prevColour = nextDesign = prevDesign = false;
	// To see how many parts are selected
	numOfParts = 0;
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

	RenderMeshOnScreen(meshList[GEO_MENU], 40, 60, 0.5, 0.5);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "INVENTORY", Color(0, 0, 0), 4, 5, 14);
	RenderTextOnScreen(meshList[GEO_TEXT], "CUSTOMISE YOUR CAR TO", Color(0, 0, 0), 2.5, 5, 21);
	RenderTextOnScreen(meshList[GEO_TEXT], "TEST DRIVE", Color(0, 0, 0), 2.5, 10, 20);
	// Parts that do not need any customization
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF SUSPENSION PARTS: 1", Color(0, 0, 0), 1.7, 0, 27); // need 1 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF ENGINE PARTS: 1", Color(0, 0, 0), 1.7, 0, 24); // need 1 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF HEADLIGHT PARTS: 2", Color(0, 0, 0), 1.7, 0, 21); // need 2 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF BRAKELIGHT PARTS: 2", Color(0, 0, 0), 1.7, 0, 18); // need 2 to be counted

	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF WINDOW PARTS: 6", Color(0, 0, 0), 1.7, 0, 15); // need 6 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF WHEEL PARTS: 4", Color(0, 0, 0), 1.7, 0, 12); // need 4 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF CARFRAME PARTS: 1", Color(0, 0, 0), 1.7, 0, 9); // need 1 to be counted

	RenderTextOnScreen(meshList[GEO_TEXT], " PRESS 'SPACE' TO SELECT", Color(0, 0, 0), 3, 1, 0);

	if (numOfParts == 7)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "PRESS HERE TO TEST DRIVE!", Color(0, 0, 0), 3, 1, 1.5);
	}
	else
	{
		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(numOfParts) + " /7 CAR PARTS SELECTED", Color(0, 0, 0), 2.5, 5, 2);
	}

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
					numOfParts++;
				}
				else if (numberBox1)
				{
					meshList[GEO_CHECKBOX1] = MeshBuilder::GenerateQuad("checkBox1", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox1 = false;
					numOfParts--;
				}
			}
			if (numberBox == 2)
			{
				if (!numberBox2)
				{
					meshList[GEO_CHECKBOX2]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX2], CHECKBOX2, true, true);
					numberBox2 = true;
					numOfParts++;
				}
				else if (numberBox2)
				{
					meshList[GEO_CHECKBOX2] = MeshBuilder::GenerateQuad("checkBox2", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox2 = false;
					numOfParts--;
				}
			}
			if (numberBox == 3)
			{
				if (!numberBox3)
				{
					meshList[GEO_CHECKBOX3]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX3], CHECKBOX3, true, true);
					numberBox3 = true;
					numOfParts++;
				}
				else if (numberBox3)
				{
					meshList[GEO_CHECKBOX3] = MeshBuilder::GenerateQuad("checkBox3", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox3 = false;
					numOfParts--;
				}
			}
			if (numberBox == 4)
			{
				if (!numberBox4)
				{
					meshList[GEO_CHECKBOX4]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX4], CHECKBOX4, true, true);
					numberBox4 = true;
					numOfParts++;
				}
				else if (numberBox4)
				{
					meshList[GEO_CHECKBOX4] = MeshBuilder::GenerateQuad("checkBox4", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox4 = false;
					numOfParts--;
				}
			}
			if (numberBox == 5)
			{
				if (!numberBox5)
				{
					meshList[GEO_CHECKBOX5]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX5], CHECKBOX5, true, true);
					numberBox5 = true;
					numOfParts++;
				}
				else if (numberBox5)
				{
					meshList[GEO_CHECKBOX5] = MeshBuilder::GenerateQuad("checkBox5", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox5 = false;
					numOfParts--;
				}
			}
			if (numberBox == 6) 
			{
				if (!numberBox6) 
				{
					meshList[GEO_CHECKBOX6]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX6], CHECKBOX6, true, true);
					numberBox6 = true;
					numOfParts++;
				}
				else if (numberBox6)
				{
					meshList[GEO_CHECKBOX6] = MeshBuilder::GenerateQuad("checkBox6", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox6 = false;
					numOfParts--;
				}
			}
			if (numberBox == 7)
			{
				if (!numberBox7)
				{
					meshList[GEO_CHECKBOX7]->textureID = LoadTGA("Image//enoughPartsTexture.tga");
					RenderOBJ(meshList[GEO_CHECKBOX7], CHECKBOX7, true, true);
					numberBox7 = true;
					numOfParts++;
				}
				else if (numberBox7)
				{
					meshList[GEO_CHECKBOX7] = MeshBuilder::GenerateQuad("checkBox7", Color(0, 0, 0), 0.4f, 0.4f);
					numberBox7 = false;
					numOfParts--;
				}
			}
			inCheckBox = false;
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
		RenderOBJ(meshList[GEO_CHECKBOX7], CHECKBOX7, true, true);
	}
	// For the choosing of designs and colours of the car parts 
	if (numberBox == 5 && numberBox5)
	{
		//Window
		RenderTextOnScreen(meshList[GEO_TEXT], " CHOOSE YOUR", Color(0, 0, 0), 2, 27, 20);
		RenderTextOnScreen(meshList[GEO_TEXT], "COLOUR:", Color(0, 0, 0), 2, 30, 19);
		RenderTextOnScreen(meshList[GEO_TEXT], "<", Color(0, 0, 0), 3, 20.5, 11);
		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(windowColourNum), Color(0, 0, 0), 3, 22, 11);
		RenderTextOnScreen(meshList[GEO_TEXT], ">", Color(0, 0, 0), 3, 23.5, 11);
		RenderTextOnScreen(meshList[GEO_TEXT], "NEXT", Color(0, 0, 0), 1.5, 49, 22.5);
		RenderTextOnScreen(meshList[GEO_TEXT], "PREV", Color(0, 0, 0), 1.5, 36, 22.5);

		if (nextButton)
		{
			if (windowColourNum < 2)
			{
				windowColourNum++;
			}
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(windowColourNum), Color(0, 0, 0), 4, 16.5, 8.1);
			nextButton = false;
		}
		if (prevButton)
		{
			if (windowColourNum > 1)
			{
				windowColourNum--;
			}
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(windowColourNum), Color(0, 0, 0), 4, 16.5, 8.1);
			prevButton = false;
		}
		// To show the texture of the window colour
		if (windowColourNum == 1)
		{
			RenderOBJ(meshList[GEO_WINDOWCOLOUR1], WINDOWCOLOUR1, true, true);
		}
		else if (windowColourNum == 2)
		{
			RenderOBJ(meshList[GEO_WINDOWCOLOUR2], WINDOWCOLOUR2, true, true);
		}
	}
	else if (numberBox == 6 && numberBox6) 
	{
		// Wheels design
		RenderTextOnScreen(meshList[GEO_TEXT], " CHOOSE YOUR", Color(0, 0, 0), 2, 27, 20); 
		RenderTextOnScreen(meshList[GEO_TEXT], "DESIGN:", Color(0, 0, 0), 2, 30, 19); 
		RenderTextOnScreen(meshList[GEO_TEXT], "<", Color(0, 0, 0), 3, 20.5, 11);
		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(wheelDesignNum), Color(0, 0, 0), 3, 22, 11);
		RenderTextOnScreen(meshList[GEO_TEXT], ">", Color(0, 0, 0), 3, 23.5, 11);
		RenderTextOnScreen(meshList[GEO_TEXT], "NEXT", Color(0, 0, 0), 1.5, 49, 22.5);
		RenderTextOnScreen(meshList[GEO_TEXT], "PREV", Color(0, 0, 0), 1.5, 36, 22.5);

		if (nextButton)
		{
			if (wheelDesignNum < 3)
			{
				wheelDesignNum++;
			}
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(wheelDesignNum), Color(0, 0, 0), 4, 16.5, 8.1);
			nextButton = false;
		}
		if (prevButton)
		{
			if (wheelDesignNum > 1)
			{
				wheelDesignNum--;
			}
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(wheelDesignNum), Color(0, 0, 0), 4, 16.5, 8.1);
			prevButton = false;
		}
		// To show the texture of the wheel design
		if (wheelDesignNum == 1)
		{
			RenderOBJ(meshList[GEO_WHEELDESIGN1], WHEELDESIGN1, true, true);
		}
		else if (wheelDesignNum == 2)
		{
			RenderOBJ(meshList[GEO_WHEELDESIGN2], WHEELDESIGN2, true, true);
		}
		else if (wheelDesignNum == 3)
		{
			RenderOBJ(meshList[GEO_WHEELDESIGN3], WHEELDESIGN3, true, true);
		}
	}
	else if (numberBox == 7 && numberBox7) 
	{
		// Choose carframe colours
		RenderTextOnScreen(meshList[GEO_TEXT], " CHOOSE YOUR", Color(0, 0, 0), 2, 27, 22.5); 
		RenderTextOnScreen(meshList[GEO_TEXT], "COLOUR:", Color(0, 0, 0), 2, 30, 21.5); 
		RenderTextOnScreen(meshList[GEO_TEXT], "<", Color(0, 0, 0), 3, 20.5, 13);
		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(cfColourNum), Color(0, 0, 0), 3, 22, 13);
		RenderTextOnScreen(meshList[GEO_TEXT], ">", Color(0, 0, 0), 3, 23.5, 13);
		RenderTextOnScreen(meshList[GEO_TEXT], "NEXT", Color(0, 0, 0), 1.5, 49, 26.5);
		RenderTextOnScreen(meshList[GEO_TEXT], "PREV", Color(0, 0, 0), 1.5, 36, 26.5);
		// Choose carframe design
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

		// To show the texture of the carframe design
		if (cfDesignNum == 1)
		{
			RenderOBJ(meshList[GEO_CFDESIGN1], CFDESIGN1, true, true);
		}
		else if (cfDesignNum == 2)
		{
			RenderOBJ(meshList[GEO_CFDESIGN2], CFDESIGN2, true, true);
		}
		else if (cfDesignNum == 3)
		{
			RenderOBJ(meshList[GEO_CFDESIGN3], CFDESIGN3, true, true);
		}
		// To show the texture of the carframe colour
		if (cfColourNum == 1)
		{
			RenderOBJ(meshList[GEO_CFCOLOUR1], CFCOLOUR1, true, true);
		}
		else if (cfColourNum == 2)
		{
			RenderOBJ(meshList[GEO_CFCOLOUR2], CFCOLOUR2, true, true);
		}
		else if (cfColourNum == 3)
		{
			RenderOBJ(meshList[GEO_CFCOLOUR3], CFCOLOUR3, true, true);
		}
		else if (cfColourNum == 4)
		{
			RenderOBJ(meshList[GEO_CFCOLOUR4], CFCOLOUR4, true, true);
		}
	}
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

bool selectionMenu::CheckCollision(object& one)
{
	if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -7.915 && one.pos.y <= -7.408)) // Checkbox1
	{
		numberBox = 1;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -8.595 && one.pos.y <= -8.168)) // Checkbox2
	{
		numberBox = 2;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -9.268 && one.pos.y <= -8.928)) // Checkbox3
	{
		numberBox = 3;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -10.033 && one.pos.y <= -9.609)) // Checkbox4
	{
		numberBox = 4;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -10.670 && one.pos.y <= -10.328)) // Checkbox5
	{
		numberBox = 5;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -11.431 && one.pos.y <= -11.012)) // Checkbox6
	{
		numberBox = 6;
		inCheckBox = true;
		return true;
	}
	else if ((one.pos.x >= 1.312 && one.pos.x <= 1.734) && (one.pos.y >= -12.111 && one.pos.y <= -11.685)) // Checkbox7
	{
		numberBox = 7;
		inCheckBox = true;
		return true;
	}

	// Check whether collided with prev and next buttons for windows
	else if (numberBox5 && (one.pos.x >= 1.882 && one.pos.x <= 2.733) && (one.pos.y >= -9.491 && one.pos.y <= -9.236))
	{
		prevButton = true;
	}
	else if (numberBox5 && (one.pos.x >= 4.607 && one.pos.x <= 5.374) && (one.pos.y >= -9.491 && one.pos.y <= -9.236))
	{
		nextButton = true;
	}
	// Check whether collided with prev and next buttons for wheels
	else if (numberBox6 && (one.pos.x >= 1.882 && one.pos.x <= 2.733) && (one.pos.y >= -9.491 && one.pos.y <= -9.236))
	{
		prevButton = true;
	}
	else if (numberBox6 && (one.pos.x >= 4.607 && one.pos.x <= 5.374) && (one.pos.y >= -9.491 && one.pos.y <= -9.236))
	{
		nextButton = true;
	}
	// Check whether collided with prev and next buttons for carframe
	else if (numberBox7 && (one.pos.x >= 1.882 && one.pos.x <= 2.733) && (one.pos.y >= -8.702 && one.pos.y <= -8.447)) 
	{
		prevColour = true;
	}
	else if (numberBox7 && (one.pos.x >= 4.607 && one.pos.x <= 5.374) && (one.pos.y >= -8.702 && one.pos.y <= -8.447))
	{
		nextColour = true;
	}
	else if (numberBox7 && (one.pos.x >= 1.882 && one.pos.x <= 2.733) && (one.pos.y >= -11.347 && one.pos.y <= -11.091)) 
	{
		prevDesign = true;
	}
	else if (numberBox7 && (one.pos.x >= 4.607 && one.pos.x <= 5.374) && (one.pos.y >= -11.347 && one.pos.y <= -11.091))
	{
		nextDesign = true;
	}
	return false;
}

void selectionMenu::doCheckBoxCollision()
{
	// Check whether target has collision with any checkboxes
	CheckCollision(target);
}
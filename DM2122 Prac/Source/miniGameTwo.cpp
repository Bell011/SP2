#include "miniGameTwo.h"
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

miniGameTwo::miniGameTwo()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

miniGameTwo::~miniGameTwo()
{

}

void miniGameTwo::Init()
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

	//init character
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	meshList[GEO_BG] = MeshBuilder::GenerateQuad("BG", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BG]->textureID = LoadTGA("Image//top.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_CHAR] = MeshBuilder::GenerateQuad("char", Color(1, 1, 1), 36.f, 45.f);
	meshList[GEO_CHAR]->textureID = LoadTGA("Image//chopper.tga");

	meshList[GEO_GAMESCREEN] = MeshBuilder::GenerateQuad("gameScreen", Color(0, 0, 0), 15.f, 10.f);

	//meshList[GEO_TARGET] = MeshBuilder::GenerateCuboid("target", Color(0, 0, 0), 1.f, 1.f, 1.f);
	meshList[GEO_TARGET] = MeshBuilder::GenerateOBJ("target", "OBJ//target.obj");
	target.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, 0.111);
	meshList[GEO_TARGET]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_TARGET]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TARGET]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TARGET]->material.kShininess = 1.f;

	movex = 0; movey = -10; movez = 0; 

	meshList[GEO_BLUEPRINT1] = MeshBuilder::GenerateOBJ("BluePrint1", "OBJ//g2bp1.obj");
	meshList[GEO_BLUEPRINT1]->textureID = LoadTGA("Image//g2bp1.tga");
	BLUEPRINT1.Translate = Vector3(0, -13, -4.5);
	//BLUEPRINT1.Scale = Vector3(1, 1, 1);
	bluePrintInitialPos1 = BLUEPRINT1.Translate;
	blueprint1.pos = Vector3(BLUEPRINT1.Translate.x, BLUEPRINT1.Translate.y, BLUEPRINT1.Translate.z);
	meshList[GEO_BLUEPRINT1]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BLUEPRINT1]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BLUEPRINT1]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BLUEPRINT1]->material.kShininess = 1.f;

	meshList[GEO_SPACING1] = MeshBuilder::GenerateOBJ("spacing1", "OBJ//g2bp1.obj");
	SPACING1.Translate = Vector3(4.5, -7.7, -4.5);
	SPACING1.Scale = Vector3(1.2, 1.2, 1.2);
	spacing1.pos = Vector3(SPACING1.Translate.x, SPACING1.Translate.y, SPACING1.Translate.z);
	meshList[GEO_SPACING1]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SPACING1]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SPACING1]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SPACING1]->material.kShininess = 1.f;

	meshList[GEO_BLUEPRINT2] = MeshBuilder::GenerateOBJ("BluePrint2", "OBJ//g2bp2.obj");
	meshList[GEO_BLUEPRINT2]->textureID = LoadTGA("Image//g2bp2.tga");
	BLUEPRINT2.Translate = Vector3(-4.5, -13, -4.5);
	//BLUEPRINT2.Scale = Vector3(1, 1, 1);
	bluePrintInitialPos2 = BLUEPRINT2.Translate;
	blueprint2.pos = Vector3(BLUEPRINT2.Translate.x, BLUEPRINT2.Translate.y, BLUEPRINT2.Translate.z);
	meshList[GEO_BLUEPRINT2]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BLUEPRINT2]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BLUEPRINT2]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BLUEPRINT2]->material.kShininess = 1.f;

	meshList[GEO_SPACING2] = MeshBuilder::GenerateOBJ("spacing2", "OBJ//g2bp2.obj");
	SPACING2.Translate = Vector3(0, -8, -4.5);
	SPACING2.Scale = Vector3(1.2, 1.2, 1.2);
	spacing2.pos = Vector3(SPACING2.Translate.x, SPACING2.Translate.y, SPACING2.Translate.z);
	meshList[GEO_SPACING2]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SPACING2]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SPACING2]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SPACING2]->material.kShininess = 1.f;

	meshList[GEO_BLUEPRINT3] = MeshBuilder::GenerateOBJ("BluePrint3", "OBJ//g2bp3.obj");
	meshList[GEO_BLUEPRINT3]->textureID = LoadTGA("Image//g2bp3.tga");
	BLUEPRINT3.Translate = Vector3(4.5, -13, -4.5);
	//BLUEPRINT3.Scale = Vector3(1, 1, 1);
	bluePrintInitialPos3 = BLUEPRINT3.Translate;
	blueprint3.pos = Vector3(BLUEPRINT3.Translate.x, BLUEPRINT3.Translate.y, BLUEPRINT3.Translate.z);
	meshList[GEO_BLUEPRINT3]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BLUEPRINT3]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BLUEPRINT3]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BLUEPRINT3]->material.kShininess = 1.f;

	meshList[GEO_SPACING3] = MeshBuilder::GenerateOBJ("spacing3", "OBJ//g2bp3.obj");
	SPACING3.Translate = Vector3(-4.7, -8.7, -4.5);
	SPACING3.Scale = Vector3(1.2, 1.2, 1.2);
	spacing3.pos = Vector3(SPACING3.Translate.x, SPACING3.Translate.y, SPACING3.Translate.z);
	meshList[GEO_SPACING3]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SPACING3]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SPACING3]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SPACING3]->material.kShininess = 1.f;


	pickBluePrint = false;
	matchBluePrint = false;
	followingTarget = false;
	bp1 = bp2 = bp3 = false;
	space1 = space2 = space3 = false;
	amountOfMoney = 0;
	numberOfBP = 0;
}

void miniGameTwo::Update(double dt)
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
	std::cout << target.pos.x << target.pos.y;
	std::cout << numberOfBP;
	if (Application::IsKeyPressed(VK_SPACE))
	{
		static float lastTime = 0.0f;
		float currentTime = GetTickCount() * 0.001f;

		if (currentTime - lastTime > 1.0f)
		{
			lastTime = currentTime;

			if (!pickBluePrint)
			{
				doPickUpCollision(); // Check which blueprint the target collided with

				if (bp1 || bp2 || bp3) // So long as any blueprint is picked up
				{
					pickBluePrint = true;
				}
				else
				{
					pickBluePrint = false;
				}
			}
			else if (pickBluePrint) // Target is moving with the blueprint
			{
				doMatchingCollision(); // To check which space the blueprint collided with
				followingTarget = true;
				if (space1 || space2 || space3) // So long as any spacing is filled up
				{
					matchBluePrint = true;
				}
				else
				{
					matchBluePrint = false;
				}
			}
		}
	}
}

void miniGameTwo::RenderOBJ(Mesh* mesh, TRS& trs, bool end, bool enableLight)
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

void miniGameTwo::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	//Render in order from Background to Foreground
	//RenderSkybox();
	RenderMeshOnScreen(meshList[GEO_BG], 80, 60, 0.5, 0.5);

	modelStack.PushMatrix();
	modelStack.Translate(0, -9, -5);
	RenderMesh(meshList[GEO_GAMESCREEN], false);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT], "N0. OF BLUEPRINTS MATCHED:" + std::to_string(numberOfBP) + "/3", Color(0, 0, 0), 2, 0, 3);
	RenderTextOnScreen(meshList[GEO_TEXT], "MONEY COLLECTED: " + std::to_string(amountOfMoney), Color(0, 0, 0), 2, 0, 2);
	RenderTextOnScreen(meshList[GEO_TEXT], "PRESS 'SPACE' TO PICK AND DROP", Color(0, 0, 0), 2.5, 0, 0);

	TARGET.Translate = Vector3(movex, movey, movez);
	target.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z);
	RenderOBJ(meshList[GEO_TARGET], TARGET, true, true);
	if (!pickBluePrint)
	{
		if (!matchBluePrint)
		{
			// Failed to pick up blueprint
			// Everything remains at their original position
			RenderOBJ(meshList[GEO_BLUEPRINT1], BLUEPRINT1, true, true);
			RenderOBJ(meshList[GEO_SPACING1], SPACING1, true, true);
			RenderOBJ(meshList[GEO_BLUEPRINT2], BLUEPRINT2, true, true);
			RenderOBJ(meshList[GEO_SPACING2], SPACING2, true, true);
			RenderOBJ(meshList[GEO_BLUEPRINT3], BLUEPRINT3, true, true);
			RenderOBJ(meshList[GEO_SPACING3], SPACING3, true, true);
		}
	}
	else if (pickBluePrint) // Player succeeded in picking up blueprint
	{
		if (followingTarget) // Attempting to match blueprint and spacing
		{
			if (matchBluePrint) // Blueprint match spacing
			{
				// Blueprint fills up spacing
				if (space1)
				{
					BLUEPRINT1.Translate = Vector3(SPACING1.Translate.x, SPACING1.Translate.y, SPACING1.Translate.z);
					BLUEPRINT1.Scale = Vector3(SPACING1.Scale.x, SPACING1.Scale.y, SPACING1.Scale.z);
					RenderOBJ(meshList[GEO_BLUEPRINT1], BLUEPRINT1, true, true);
					RenderOBJ(meshList[GEO_BLUEPRINT2], BLUEPRINT2, true, true);
					RenderOBJ(meshList[GEO_SPACING2], SPACING2, true, true);
					RenderOBJ(meshList[GEO_BLUEPRINT3], BLUEPRINT3, true, true);
					RenderOBJ(meshList[GEO_SPACING3], SPACING3, true, true);
					space1 = false;
					numberOfBP++;
				}
				else if (space2)
				{
					BLUEPRINT2.Translate = Vector3(SPACING2.Translate.x, SPACING2.Translate.y, SPACING2.Translate.z);
					BLUEPRINT2.Scale = Vector3(SPACING2.Scale.x, SPACING2.Scale.y, SPACING2.Scale.z);
					RenderOBJ(meshList[GEO_BLUEPRINT2], BLUEPRINT2, true, true);
					RenderOBJ(meshList[GEO_BLUEPRINT1], BLUEPRINT1, true, true);
					RenderOBJ(meshList[GEO_SPACING1], SPACING1, true, true);
					RenderOBJ(meshList[GEO_BLUEPRINT3], BLUEPRINT3, true, true);
					RenderOBJ(meshList[GEO_SPACING3], SPACING3, true, true);
					space2 = false;
					numberOfBP++;
				}
				else if (space3)
				{
					BLUEPRINT3.Translate = Vector3(SPACING3.Translate.x, SPACING3.Translate.y, SPACING3.Translate.z);
					BLUEPRINT3.Scale = Vector3(SPACING3.Scale.x, SPACING3.Scale.y, SPACING3.Scale.z);
					RenderOBJ(meshList[GEO_BLUEPRINT3], BLUEPRINT3, true, true);
					RenderOBJ(meshList[GEO_BLUEPRINT1], BLUEPRINT1, true, true);
					RenderOBJ(meshList[GEO_SPACING1], SPACING1, true, true);
					RenderOBJ(meshList[GEO_BLUEPRINT2], BLUEPRINT2, true, true);
					RenderOBJ(meshList[GEO_SPACING2], SPACING2, true, true);
					space3 = false;
					numberOfBP++;
				}
			}
			else if (!matchBluePrint) // Blueprints does not match spacing
			{
				// Blueprints returns to its original position
				BLUEPRINT1.Translate = bluePrintInitialPos1;
				blueprint1.pos = Vector3(BLUEPRINT1.Translate.x, BLUEPRINT1.Translate.y, BLUEPRINT1.Translate.z);
				RenderOBJ(meshList[GEO_BLUEPRINT1], BLUEPRINT1, true, true);
				RenderOBJ(meshList[GEO_SPACING1], SPACING1, true, true);
				BLUEPRINT2.Translate = bluePrintInitialPos2;
				blueprint2.pos = Vector3(BLUEPRINT2.Translate.x, BLUEPRINT2.Translate.y, BLUEPRINT2.Translate.z);
				RenderOBJ(meshList[GEO_BLUEPRINT2], BLUEPRINT2, true, true);
				RenderOBJ(meshList[GEO_SPACING2], SPACING2, true, true);
				BLUEPRINT3.Translate = bluePrintInitialPos3;
				blueprint3.pos = Vector3(BLUEPRINT3.Translate.x, BLUEPRINT3.Translate.y, BLUEPRINT3.Translate.z);
				RenderOBJ(meshList[GEO_BLUEPRINT3], BLUEPRINT3, true, true);
				RenderOBJ(meshList[GEO_SPACING3], SPACING3, true, true);
			}
			pickBluePrint = false;
			matchBluePrint = false;
			followingTarget = false;
			bp1 = false;
			bp2 = false;
			bp3 = false;
		}
		else if (!followingTarget)
		{
			// Blueprint will follow target
			if (bp1)
			{
				BLUEPRINT1.Translate = Vector3(movex, movey, movez);
				blueprint1.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z);
				RenderOBJ(meshList[GEO_BLUEPRINT1], BLUEPRINT1, true, true);
				RenderOBJ(meshList[GEO_SPACING1], SPACING1, true, true);
				RenderOBJ(meshList[GEO_BLUEPRINT2], BLUEPRINT2, true, true);
				RenderOBJ(meshList[GEO_SPACING2], SPACING2, true, true);
				RenderOBJ(meshList[GEO_BLUEPRINT3], BLUEPRINT3, true, true);
				RenderOBJ(meshList[GEO_SPACING3], SPACING3, true, true);
			}
			else if (bp2)
			{
				BLUEPRINT2.Translate = Vector3(movex, movey, movez);
				blueprint2.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z);
				RenderOBJ(meshList[GEO_BLUEPRINT2], BLUEPRINT2, true, true);
				RenderOBJ(meshList[GEO_SPACING2], SPACING2, true, true);
				RenderOBJ(meshList[GEO_BLUEPRINT1], BLUEPRINT1, true, true);
				RenderOBJ(meshList[GEO_SPACING1], SPACING1, true, true);
				RenderOBJ(meshList[GEO_BLUEPRINT3], BLUEPRINT3, true, true);
				RenderOBJ(meshList[GEO_SPACING3], SPACING3, true, true);
			}
			else if (bp3)
			{
				BLUEPRINT3.Translate = Vector3(movex, movey, movez);
				blueprint3.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z);
				RenderOBJ(meshList[GEO_BLUEPRINT3], BLUEPRINT3, true, true);
				RenderOBJ(meshList[GEO_SPACING3], SPACING3, true, true);
				RenderOBJ(meshList[GEO_BLUEPRINT1], BLUEPRINT1, true, true);
				RenderOBJ(meshList[GEO_SPACING1], SPACING1, true, true);
				RenderOBJ(meshList[GEO_BLUEPRINT2], BLUEPRINT2, true, true);
				RenderOBJ(meshList[GEO_SPACING2], SPACING2, true, true);
			}
		}
	}
	if (numberOfBP == 3) // All spaces have been filled up
	{
		amountOfMoney += 100;
		numberOfBP = 0;
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
void miniGameTwo::RenderMeshOnScreen(Mesh* mesh, float sizex, float sizey, float x, float y) {
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

void miniGameTwo::Exit()
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

void miniGameTwo::RenderMesh(Mesh* mesh, bool enableLight)
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

void miniGameTwo::RenderText(Mesh* mesh, std::string text, Color color)
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

void miniGameTwo::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void miniGameTwo::CalculateFrameRate()
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

void miniGameTwo::RenderSkybox()
{
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 0.f, -50.f);
	modelStack.Scale(100.f, 100.f, 100.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
}

bool miniGameTwo::CheckCollision(object& one, object& two)
{
	if (!pickBluePrint) // To check whether target is in the blueprint spaces
	{
		if ((one.pos.x >= -1.104 && one.pos.x <= 1.113) && (one.pos.y >= -12.133 && one.pos.y <= -10.512)) // Blueprint1
		{
			bp1 = true;
			return true;
		}
		else if ((one.pos.x >= -4.031 && one.pos.x <= -2.239) && (one.pos.y >= -12.130 && one.pos.y <= -10.333)) // Blueprint2
		{
			bp2 = true;
			return true;
		}
		else if ((one.pos.x >= 2.224 && one.pos.x <= 4.012) && (one.pos.y >= -12.047 && one.pos.y <= -9.741)) // Blueprint3
		{
			bp3 = true;
			return true;
		}
	}
	if (pickBluePrint) // To check whether blueprint is in the spacing
	{
		// Ensure that only the respective blueprints will get into the respective spaces
		if (bp1 && (one.pos.x >= 1.769 && one.pos.x <= 4.507) && (one.pos.y >= -8.455 && one.pos.y <= -6.495)) // Spacing1
		{
			space1 = true;
			return true;
		}
		else if (bp2 && (one.pos.x >= -1.031 && one.pos.x <= 1.100) && (one.pos.y >= -8.627 && one.pos.y <= -6.496)) // Spacing2
		{
			space2 = true;
			return true;
		}
		else if (bp3 && (one.pos.x >= -4.355 && one.pos.x <= -2.142) && (one.pos.y >= -9.136 && one.pos.y <= -6.240)) // Spacing3
		{
			space3 = true;
			return true;
		}
	}
	return false;
}

void miniGameTwo::doPickUpCollision()
{
	// Check which blueprints have collision with the target
	CheckCollision(target, blueprint1);
	CheckCollision(target, blueprint2);
	CheckCollision(target, blueprint3);
}

void miniGameTwo::doMatchingCollision()
{
	// Check which spaces have collision with the blueprint
	CheckCollision(blueprint1, spacing1);
	CheckCollision(blueprint2, spacing2);
	CheckCollision(blueprint3, spacing3);
}
#include "miniGameTwo.h"
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
	
	meshList[GEO_GAMESCREEN] = MeshBuilder::GenerateQuad("gameScreen", Color(1, 1, 1), 13.f, 10.f);
	//meshList[GEO_TARGET] = MeshBuilder::GenerateQuad("target", Color(1, 1, 1), 1.f, 1.f);

	meshList[GEO_TARGET] = MeshBuilder::GenerateCuboid("target", Color(0, 0, 0), 1.f, 1.f, 1.f);
	TARGET.Translate = Vector3(-3, -10, -4.5);
	TARGET.Scale = Vector3(0.1, 0.1, 0.1);
	target.size = Vector3(0.5 * TARGET.Scale.x, 0.5 * TARGET.Scale.y, 0.5 * TARGET.Scale.z);
	target.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z);
	meshList[GEO_TARGET]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_TARGET]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TARGET]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TARGET]->material.kShininess = 1.f;

	//meshList[GEO_TARGET]->textureID = LoadTGA("Image//target.tga");
	movex = 0; movey = -10; movez = 0;

	meshList[GEO_BLUEPRINT] = MeshBuilder::GenerateCuboid("TargetCube", Color(1, 0, 0), 1.f, 1.f, 1.f);
	BLUEPRINT.Translate = Vector3(2, -10, -4.5);
	BLUEPRINT.Scale = Vector3(1, 1, 1);
	bluePrintInitialPos = BLUEPRINT.Translate;
	blueprint.size = Vector3(0.5 * BLUEPRINT.Scale.x, 0.5 * BLUEPRINT.Scale.y, 0.5 * BLUEPRINT.Scale.z);
	blueprint.pos = Vector3(BLUEPRINT.Translate.x, BLUEPRINT.Translate.y, BLUEPRINT.Translate.z);
	meshList[GEO_BLUEPRINT]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_BLUEPRINT]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BLUEPRINT]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BLUEPRINT]->material.kShininess = 1.f;

	// Space for the blueprints
	meshList[GEO_SPACING] = MeshBuilder::GenerateCuboid("Testing", Color(0, 1, 0), 1.f, 1.f, 1.f);
	SPACING.Translate = Vector3(-2, -10, -4.5);
	SPACING.Scale = Vector3(2.5, 2.5, 2.5);
	spacing.size = Vector3(0.5 * SPACING.Scale.x, 0.5 * SPACING.Scale.y, 0.5 * SPACING.Scale.z);
	spacing.pos = Vector3(SPACING.Translate.x, SPACING.Translate.y, SPACING.Translate.z);
	meshList[GEO_SPACING]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SPACING]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SPACING]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SPACING]->material.kShininess = 1.f;

	pickBluePrint = false;
	matchBluePrint = false;
	followingTarget = false;
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
	//doPickUpCollision();
	//doMatchingCollision();
 	if (Application::IsKeyPressed(VK_SPACE))
	{
		static float lastTime = 0.0f;
		float currentTime = GetTickCount() * 0.001f;
		
		if (currentTime - lastTime > 1.0f)
		{
			lastTime = currentTime;
			
			if (!pickBluePrint)
			{
				if (CheckCollision(target, blueprint)) // Succeed in picking up blueprint
				{
					pickBluePrint = true;
				}
				else if (!CheckCollision(target, blueprint)) // Failed in picking up blueprint
				{
					pickBluePrint = false;
				}
			}
			else if (pickBluePrint) // Target is moving with the blueprint
			{
				followingTarget = true;
				if (!matchBluePrint && CheckCollision(blueprint, spacing)) // Blueprint match space
				{
					matchBluePrint = true;
				}
				else if (!matchBluePrint && !CheckCollision(blueprint, spacing)) // Blueprint does not match space
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
		modelStack.Translate(1.5, -10, -5);
		RenderMesh(meshList[GEO_GAMESCREEN], false);
	modelStack.PopMatrix();

	//RenderMeshOnScreen(meshList[GEO_CHAR], 0.5, 0.75, 40, 30);
	//RenderTextOnScreen(meshList[GEO_TEXT], "Hi", Color(0, 1, 0), 5, 0, 0);

	RenderTextOnScreen(meshList[GEO_TEXT], "WELCOME TO MINI-GAME 2:", Color(0, 0, 0), 2, 0, 29);
	RenderTextOnScreen(meshList[GEO_TEXT], "MATCHING BLUEPRINTS", Color(0, 0, 0), 2, 0, 28);
	RenderTextOnScreen(meshList[GEO_TEXT], "NO. OF", Color(0, 0, 0), 1.5, 0, 26);
	RenderTextOnScreen(meshList[GEO_TEXT], "BLUEPRINTS", Color(0, 0, 0), 1.5, 0, 25);
	RenderTextOnScreen(meshList[GEO_TEXT], "MATCHED: ", Color(0, 0, 0), 1.5, 0, 24);

	TARGET.Translate = Vector3(movex, movey, movez);
	target.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z);
	RenderOBJ(meshList[GEO_TARGET], TARGET, true, true); 
	if (!pickBluePrint) 
	{
		if (!matchBluePrint)
		{
			// Failed to pick up blueprint
			// Everything remains at their original position
			RenderOBJ(meshList[GEO_BLUEPRINT], BLUEPRINT, true, true);
			RenderOBJ(meshList[GEO_SPACING], SPACING, true, true); 
		}
	}
	else if (pickBluePrint) // Player succeeded in picking up blueprint
	{
		if (followingTarget) // Attempting to match blueprint and spacing
		{
			if (matchBluePrint) // Blueprint match spacing
			{
				// Blueprint fills up spacing
				BLUEPRINT.Translate = Vector3(SPACING.Translate.x, SPACING.Translate.y, SPACING.Translate.z);
				BLUEPRINT.Scale = Vector3(SPACING.Scale.x, SPACING.Scale.y, SPACING.Scale.z);
				RenderOBJ(meshList[GEO_BLUEPRINT], BLUEPRINT, true, true);
			}
			else if (!matchBluePrint) // Blueprint does not match spacing
			{
				// Blueprint returns to its original position
				BLUEPRINT.Translate = bluePrintInitialPos;
				blueprint.pos = Vector3(BLUEPRINT.Translate.x, BLUEPRINT.Translate.y, BLUEPRINT.Translate.z);
				RenderOBJ(meshList[GEO_BLUEPRINT], BLUEPRINT, true, true); 
				RenderOBJ(meshList[GEO_SPACING], SPACING, true, true);
			}
		}
		else if (!followingTarget)
		{
			// Blueprint will follow target
			BLUEPRINT.Translate = Vector3(movex, movey, movez);
			blueprint.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z);
			RenderOBJ(meshList[GEO_BLUEPRINT], BLUEPRINT, true, true);
			RenderOBJ(meshList[GEO_SPACING], SPACING, true, true);
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
	if (
		(one.pos.x - one.size.x <= two.pos.x + two.size.x && one.pos.x + one.size.x >= two.pos.x - two.size.x) &&
		(one.pos.y - one.size.y <= two.pos.y + two.size.y && one.pos.y + one.size.y >= two.pos.y - two.size.y) //&&
		//(one.pos.z - one.size.z <= two.pos.z + two.size.z && one.pos.z + one.size.z >= two.pos.z - two.size.z)
		) {
		return true;
	}
	return false;
}

void miniGameTwo::doPickUpCollision() 
{
	if (CheckCollision(target, blueprint)) // Pick up
	{ 
		printf("YES\n");
	}
	else
	{
		printf("NO\n");
	}
}

void miniGameTwo::doMatchingCollision() 
{
	if (CheckCollision(blueprint, spacing)) // Match
	{
		printf("YES\n");
	}
	else
	{
		printf("NO\n");
	}
}
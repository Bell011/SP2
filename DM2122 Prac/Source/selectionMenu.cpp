#include "selectionMenu.h"
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
	target.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z);
	meshList[GEO_TARGET]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_TARGET]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TARGET]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TARGET]->material.kShininess = 1.f;

	movex = 0; movey = -10; movez = 0;

	meshList[GEO_CHECKBOX] = MeshBuilder::GenerateOBJ("checkBox", "OBJ//checkBox.obj");
	CHECKBOX.Translate = Vector3(0.5,-7.95,0);
	CHECKBOX.Scale = Vector3(0.2, 0.2, 0.2);
	checkbox.pos = Vector3(CHECKBOX.Translate.x, CHECKBOX.Translate.y, CHECKBOX.Translate.z);
	meshList[GEO_CHECKBOX]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CHECKBOX]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHECKBOX]->material.kShininess = 1.f;
	

	checkBox = false;
	checked = false;
	amountOfMoney = 0;
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
	doCheckBoxCollision();
	if (Application::IsKeyPressed(VK_SPACE))
	{
		// Attempt to check the box belonging to the car parts
		doCheckBoxCollision(); // Check whether target collided with the checkbox

		if (checkBox) // Target is in the checkbox space
		{
			if (!checked) // Checkbox has not been checked
			{
				checked = true;
			}
			else if (checked) // Checkbox has been checked
			{
				checked = false;
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

	TARGET.Translate = Vector3(movex, movey, movez);
	target.pos = Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z);
	RenderOBJ(meshList[GEO_TARGET], TARGET, true, true);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "INVENTORY", Color(0, 0, 0), 4, 5, 14);
	//RenderTextOnScreen(meshList[GEO_TEXT], "SELECT 1 OF EACH PART", Color(0, 0, 0), 3, 3, 17);
	//RenderTextOnScreen(meshList[GEO_TEXT], " TO BUILD YOUR CAR", Color(0, 0, 0), 3, 3, 16);
	RenderTextOnScreen(meshList[GEO_TEXT], "CUSTOMISE YOUR CAR TO", Color(0, 0, 0), 2.5, 5, 21);
	RenderTextOnScreen(meshList[GEO_TEXT], "TEST DRIVE", Color(0, 0, 0), 2.5, 10, 20);

	// Parts that do not need any customization
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF ENGINE PARTS: 1", Color(0, 0, 0), 1.7, 0, 26); // need 1 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF HEADLIGHT PARTS: 2", Color(0, 0, 0), 1.7, 0, 24); // need 2 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF BRAKELIGHT PARTS: 2", Color(0, 0, 0), 1.7, 0, 22); // need 2 to be counted

	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF WINDOW PARTS: 6", Color(0, 0, 0), 1.7, 0, 18); // need 6 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF WHEEL PARTS: 4", Color(0, 0, 0), 1.7, 0, 14); // need 4 to be counted
	RenderTextOnScreen(meshList[GEO_TEXT], " NO. OF CARFRAME PARTS: 1", Color(0, 0, 0), 1.7, 0, 10); // need 1 to be counted



	RenderTextOnScreen(meshList[GEO_TEXT], " 0/6 CAR PARTS SELECTED", Color(0, 0, 0), 2.5, 3, 2);
	RenderTextOnScreen(meshList[GEO_TEXT], " PRESS 'SPACE' TO SELECT", Color(0, 0, 0), 3, 1, 0);


	RenderOBJ(meshList[GEO_CHECKBOX], CHECKBOX, true, true);

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
	if ((one.pos.x >= -0.340 && one.pos.x <= 0.341) && (one.pos.y >= -10.338 && one.pos.y <= -9.655)) // Checkbox1
	{
		checkBox = true;
		return true;
	}
	return false;
}

void selectionMenu::doCheckBoxCollision()
{
	// Check whether target has collision with any checkboxes
	CheckCollision(target, checkbox);
	if (CheckCollision(target, checkbox))
	{
		std::cout << "YES" << std::endl;
	}
	else
	{
		std::cout << "NO" << std::endl;
	}
}

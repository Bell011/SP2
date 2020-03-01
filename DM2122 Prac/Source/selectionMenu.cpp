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

	camera.Init(Vector3(0, 5, 0), Vector3(0, 0, 0), Vector3(1, 0, 0));
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
	meshList[GEO_MENU]->textureID = LoadTGA("Image//indoor_Back.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	
	meshList[GEO_TARGET] = MeshBuilder::GenerateOBJ("target", "OBJ//target.obj");
	meshList[GEO_TARGET]->textureID = LoadTGA("Image//targetTexture.tga");
	target = new rectObj();
	movex = 0; movey = -10; movez = 0;
	target->setPos(Vector3(TARGET.Translate.x, TARGET.Translate.y, 5));

	meshList[GEO_CHECKBOX1] = MeshBuilder::GenerateQuad("checkBox1", Color(0, 0, 0), 0.4f, 0.4f); // Suspension
	CHECKBOX1.Translate = Vector3(1.5, -7.7,0);

	meshList[GEO_CHECKBOX2] = MeshBuilder::GenerateQuad("checkBox2", Color(0, 0, 0), 0.4f, 0.4f);
	CHECKBOX2.Translate = Vector3(1.5, -8.4, 0);

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

	meshList[GEO_CAR1] = MeshBuilder::GenerateOBJ("wheel1", "OBJ//car1.obj");
	meshList[GEO_CAR1]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CAR1]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR1]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR1]->material.kShininess = 1.f;
	CAR1.RotateZ = Vector4(-90, 0, 0, 1);
	CAR1.Translate = Vector3(-1, -5, 0);
	CAR1.Scale = Vector3(0.5, 0.5, 0.5);

	meshList[GEO_CAR2] = MeshBuilder::GenerateOBJ("wheel2", "OBJ//car2.obj");
	meshList[GEO_CAR2]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CAR2]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR2]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR2]->material.kShininess = 1.f;

	meshList[GEO_CAR3] = MeshBuilder::GenerateOBJ("wheel3", "OBJ//car3.obj");;
	meshList[GEO_CAR3]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CAR3]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR3]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CAR3]->material.kShininess = 1.f;
	

	// For the checkboxes
	inCheckBox = false;
	numberBox1 = numberBox2 = false;
	pressedSpace = false;
	numberBox = 0;
	// For the selection of colours and designs
	
	// To see how many parts are selected
	numOfParts = 0;
	initCars();
}

void selectionMenu::Update(double dt)
{
	
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
	std::cout << TARGET.Translate.x << " " << TARGET.Translate.x << std::endl;
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
	if (frameChoice) {
		RenderTextOnScreen(meshList[GEO_TEXT], " CHOOSE CARFRAME: Press 1-3", Color(0, 0, 0), 1.7, 0, 21); // need 1 to be counted
		if (Application::IsKeyPressed('1')) {
			checkCar1 = true;
			checkCar2 = false;
			checkCar3 = false;
		}

		if (Application::IsKeyPressed('2')) {
			checkCar2 = true;
			checkCar1 = false;
			checkCar3 = false;
		}
		if (Application::IsKeyPressed('3')) {
			checkCar3 = true;
			checkCar1 = false;
			checkCar2 = false;
		}

		if (Application::IsKeyPressed(VK_SPACE)) {
			frameChoice = false;
			texChoice = true;
		}
		RenderTextOnScreen(meshList[GEO_TEXT], " PRESS 'SPACE' TO SELECT", Color(0, 0, 0), 3, 1, 0);
	}
	if (texChoice) {
		RenderTextOnScreen(meshList[GEO_TEXT], "CHOOSE PAINT: Press 1-8", Color(0, 0, 0), 1.7, 0, 21); // need 1 to be counted
		if (checkCar1) {
			if (Application::IsKeyPressed('1'))
				meshList[GEO_CAR1]->textureID = LoadTGA("Image//car1black.tga");
			if (Application::IsKeyPressed('2'))
				meshList[GEO_CAR1]->textureID = LoadTGA("Image//car1black2.tga");
			if (Application::IsKeyPressed('3'))
				meshList[GEO_CAR1]->textureID = LoadTGA("Image//car1blue.tga");
			if (Application::IsKeyPressed('4'))
				meshList[GEO_CAR1]->textureID = LoadTGA("Image//car1blue2.tga");
			if (Application::IsKeyPressed('5'))
				meshList[GEO_CAR1]->textureID = LoadTGA("Image//car1red.tga");
			if (Application::IsKeyPressed('6'))
				meshList[GEO_CAR1]->textureID = LoadTGA("Image//car1red2.tga");
			if (Application::IsKeyPressed('7'))
				meshList[GEO_CAR1]->textureID = LoadTGA("Image//car1white.tga");
			if (Application::IsKeyPressed('8'))
				meshList[GEO_CAR1]->textureID = LoadTGA("Image//car1white2.tga");
			if (Application::IsKeyPressed(VK_RETURN)) {
				car.setCar(meshList[GEO_CAR1]);
				car.setWheel(meshList[GEO_WHEEL2]);
				car.setWheelPos(0, Vector3(0, -0.8, 1.5));
				car.setWheelPos(1, Vector3(0, 0, -3.1));
				car.setWheelPos(2, Vector3(-1.7, 0, 0));
				car.setWheelPos(3, Vector3(0, 0, 3.1));
				((TestDriveScene*)Application::getScene(8))->setCar(&car);
				Application::scenechange(8);
			}
		}
		if (checkCar2) {
			if (Application::IsKeyPressed('1'))
				meshList[GEO_CAR2]->textureID = LoadTGA("Image//car2black.tga");
			if (Application::IsKeyPressed('2'))
				meshList[GEO_CAR2]->textureID = LoadTGA("Image//car2black2.tga");
			if (Application::IsKeyPressed('3'))
				meshList[GEO_CAR2]->textureID = LoadTGA("Image//car2blue.tga");
			if (Application::IsKeyPressed('4'))
				meshList[GEO_CAR2]->textureID = LoadTGA("Image//car2blue2.tga");
			if (Application::IsKeyPressed('5'))
				meshList[GEO_CAR2]->textureID = LoadTGA("Image//car2red.tga");
			if (Application::IsKeyPressed('6'))
				meshList[GEO_CAR2]->textureID = LoadTGA("Image//car2red2.tga");
			if (Application::IsKeyPressed('7'))
				meshList[GEO_CAR2]->textureID = LoadTGA("Image//car2white.tga");
			if (Application::IsKeyPressed('8'))
				meshList[GEO_CAR2]->textureID = LoadTGA("Image//car2white2.tga");
			if (Application::IsKeyPressed(VK_RETURN)) {
				car.setCar(meshList[GEO_CAR2]);
				car.setWheel(meshList[GEO_WHEEL3]);
				car.setWheelPos(0, Vector3(1.3, -0.8, 1.5));
				car.setWheelPos(1, Vector3(0, 0, -3.1));
				car.setWheelPos(2, Vector3(-2, 0, 0));
				car.setWheelPos(3, Vector3(0, 0, 3.1));
				((TestDriveScene*)Application::getScene(8))->setCar(&car);
				Application::scenechange(8);
			}
		}
		if (checkCar3) {
			if (Application::IsKeyPressed('1'))
				meshList[GEO_CAR3]->textureID = LoadTGA("Image//car3black.tga");
			if (Application::IsKeyPressed('2'))
				meshList[GEO_CAR3]->textureID = LoadTGA("Image//car3black2.tga");
			if (Application::IsKeyPressed('3'))
				meshList[GEO_CAR3]->textureID = LoadTGA("Image//car3blue.tga");
			if (Application::IsKeyPressed('4'))
				meshList[GEO_CAR3]->textureID = LoadTGA("Image//car3blue2.tga");
			if (Application::IsKeyPressed('5'))
				meshList[GEO_CAR3]->textureID = LoadTGA("Image//car3red.tga");
			if (Application::IsKeyPressed('6'))
				meshList[GEO_CAR3]->textureID = LoadTGA("Image//car3red2.tga");
			if (Application::IsKeyPressed('7'))
				meshList[GEO_CAR3]->textureID = LoadTGA("Image//car3white.tga");
			if (Application::IsKeyPressed('8'))
				meshList[GEO_CAR3]->textureID = LoadTGA("Image//car3white2.tga");
			if (Application::IsKeyPressed(VK_RETURN)) {
				car.setCar(meshList[GEO_CAR3]);
				car.setWheel(meshList[GEO_WHEEL1]);
				car.setWheelPos(0, Vector3(0.5, -0.8, 1.75));
				car.setWheelPos(1, Vector3(0, 0, -3.3));
				car.setWheelPos(2, Vector3(-1.7, 0, 0));
				car.setWheelPos(3, Vector3(0, 0, 3.2));
				((TestDriveScene*)Application::getScene(8))->setCar(&car);
				Application::scenechange(8);
			}
		}
		RenderTextOnScreen(meshList[GEO_TEXT], " PRESS ENTER to Drive", Color(0, 0, 0), 3, 1, 0);
	
	}
	if (checkCar1) {
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
	}
	if (checkCar2) {

		RenderOBJ(meshList[GEO_CAR2], CAR1, false, true);
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
	}

	if (checkCar3) {

		RenderOBJ(meshList[GEO_CAR3], CAR1, false, true);
		WHEEL1.Translate = Vector3(0.5, -0.8, 1.75);
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
	}
	

	if (numOfParts == 7)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "PRESS HERE TO TEST DRIVE!", Color(0, 0, 0), 3, 1, 1.5);
	}


	

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
			inCheckBox = false;
		}
		pressedSpace = false;
	}
	else if (!pressedSpace)
	{
		RenderOBJ(meshList[GEO_CHECKBOX1], CHECKBOX1, true, true);
		RenderOBJ(meshList[GEO_CHECKBOX2], CHECKBOX2, true, true);

	}
	// For the choosing of designs and colours of the car parts 


	TARGET.Translate = Vector3(movex, movey, movez);
	target->setPos(Vector3(TARGET.Translate.x, TARGET.Translate.y, TARGET.Translate.z));
	RenderOBJ(meshList[GEO_TARGET], TARGET, true, true);
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

bool selectionMenu::CheckCollision(Object* one)
{
	if ((one->getPos().x >= 1.312 && one->getPos().x <= 1.734) && (one->getPos().y >= -7.915 && one->getPos().y <= -7.408)) // Checkbox1
	{
		numberBox = 1;
		inCheckBox = true;
		return true;
	}
	else if ((one->getPos().x >= 1.312 && one->getPos().x <= 1.734) && (one->getPos().y >= -8.595 && one->getPos().y <= -8.168)) // Checkbox2
	{
		numberBox = 2;
		inCheckBox = true;
		return true;
	}
	else if ((one->getPos().x >= 1.312 && one->getPos().x <= 1.734) && (one->getPos().y >= -9.268 && one->getPos().y <= -8.928)) // Checkbox3
	{
		numberBox = 3;
		inCheckBox = true;
		return true;
	}
	else if ((one->getPos().x >= 1.312 && one->getPos().x <= 1.734) && (one->getPos().y >= -10.033 && one->getPos().y <= -9.609)) // Checkbox4
	{
		numberBox = 4;
		inCheckBox = true;
		return true;
	}
	else if ((one->getPos().x >= 1.312 && one->getPos().x <= 1.734) && (one->getPos().y >= -10.670 && one->getPos().y <= -10.328)) // Checkbox5
	{
		numberBox = 5;
		inCheckBox = true;
		return true;
	}
	else if ((one->getPos().x >= 1.312 && one->getPos().x <= 1.734) && (one->getPos().y >= -11.431 && one->getPos().y <= -11.012)) // Checkbox6
	{
		numberBox = 6;
		inCheckBox = true;
		return true;
	}
	else if ((one->getPos().x >= 1.312 && one->getPos().x <= 1.734) && (one->getPos().y >= -12.111 && one->getPos().y <= -11.685)) // Checkbox7
	{
		numberBox = 7;
		inCheckBox = true;
		return true;
	}

//	// Check whether collided with prev and next buttons for windows
//	else if (numberBox5 && (one->getPos().x >= 1.882 && one->getPos().x <= 2.733) && (one->getPos().y >= -9.491 && one->getPos().y <= -9.236))
//	{
//		prevButton = true;
//	}
//	else if (numberBox5 && (one->getPos().x >= 4.607 && one->getPos().x <= 5.374) && (one->getPos().y >= -9.491 && one->getPos().y <= -9.236))
//	{
//		nextButton = true;
//	}
//	// Check whether collided with prev and next buttons for wheels
//	else if (numberBox6 && (one->getPos().x >= 1.882 && one->getPos().x <= 2.733) && (one->getPos().y >= -9.491 && one->getPos().y <= -9.236))
//	{
//		prevButton = true;
//	}
//	else if (numberBox6 && (one->getPos().x >= 4.607 && one->getPos().x <= 5.374) && (one->getPos().y >= -9.491 && one->getPos().y <= -9.236))
//	{
//		nextButton = true;
//	}
//	// Check whether collided with prev and next buttons for carframe
//	else if (numberBox7 && (one->getPos().x >= 1.882 && one->getPos().x <= 2.733) && (one->getPos().y >= -8.702 && one->getPos().y <= -8.447))
//	{
//		prevColour = true;
//	}
//	else if (numberBox7 && (one->getPos().x >= 4.607 && one->getPos().x <= 5.374) && (one->getPos().y >= -8.702 && one->getPos().y <= -8.447))
//	{
//		nextColour = true;
//	}
//	else if (numberBox7 && (one->getPos().x >= 1.882 && one->getPos().x <= 2.733) && (one->getPos().y >= -11.347 && one->getPos().y <= -11.091))
//	{
//		prevDesign = true;
//	}
//	else if (numberBox7 && (one->getPos().x >= 4.607 && one->getPos().x <= 5.374) && (one->getPos().y >= -11.347 && one->getPos().y <= -11.091))
//	{
//		nextDesign = true;
//	}
//	return false;
}

void selectionMenu::doCheckBoxCollision()
{
	// Check whether target has collision with any checkboxes
	CheckCollision(target);
}

void selectionMenu::initCars() {

}
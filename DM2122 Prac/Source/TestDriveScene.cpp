#include "TestDriveScene.h"
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

TestDriveScene::TestDriveScene()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

TestDriveScene::~TestDriveScene()
{
}

void TestDriveScene::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(player.pos.x, player.pos.y +10, player.pos.z+15), Vector3(player.pos.x,player.pos.y,player.pos.z), Vector3(0, 1, 0));

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

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	
	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	glUniform1i(m_parameters[U_NUMLIGHTS], 7); 

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//outdoor_Left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//outdoor_Right.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//outdoor_Top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//outdoor_Bottom.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//outdoor_Front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//outdoor_Back.tga");

	meshList[GEO_CHAR] = MeshBuilder::GenerateOBJ("car4", "OBJ//car4.obj");
	meshList[GEO_CHAR]->textureID = LoadTGA("Image//car4.tga");
	movex = 0;	movey = 0;	movez = 0;
	PLAYER.Translate = Vector3(0, 0, 0);
	//PLAYER.Scale = Vector3(2, 2, 2);
	PLAYER.RotateZ = Vector4(180, 0, 1, 0);
	//player.size = Vector3(length/2*CUBE.Scale.x, height/2* CUBE.Scale.y, width/2 * CUBE.Scale.z);
	/*-----------------------------------------------------------------------------------------------
	HOW TO GET LENGTH,  WIDTH, HEIGHT OF OBJECT:
	1. OPEN THE OBJ FILE USING NOTEPAD
	2. U WIlL SEE ROWS OF THESE:
		v 1.656677 3.053495 0.519560
		v 1.719025 2.998324 0.653901
		v 1.720906 3.284813 0.462875
	3. Each column represents x, y, z coord respectively
	4. Add the 2 extreme values in each column and divide by 2 to get length, height and width
		Eg. Greatest x value = 0.95		Smallest x value = -0.56;
			length = (0.95 + 0.56) / 2

	PS if i have free time i will make some function/class to read the values from the notepad file 
	   n stuff and do the math oso..but so far just manually calculate first :)
	---------------------------------------------------------------------------------------------*/
	player.size = Vector3(0.94 * PLAYER.Scale.x, 0.865 * PLAYER.Scale.y, 0.95 * PLAYER.Scale.z);
	player.pos = Vector3(CUBE.Translate.x, CUBE.Translate.y, CUBE.Translate.z);
	meshList[GEO_CHAR]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CHAR]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHAR]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CHAR]->material.kShininess = 1.f;

	meshList[GEO_CUBE] = MeshBuilder::GenerateCuboid("Cube", Color(1, 0, 0), 1.f, 1.f, 1.f);
	CUBE.Translate = Vector3(2, 0, 0);
	CUBE.Scale = Vector3(2, 2, 2);
	//cube.size = Vector3(length/2*CUBE.Scale.x, height/2* CUBE.Scale.y, width/2 * CUBE.Scale.z);
	cube.size = Vector3(0.5 * CUBE.Scale.x, 0.5 * CUBE.Scale.y, 0.5 * CUBE.Scale.z);
	cube.pos = Vector3(CUBE.Translate.x, CUBE.Translate.y, CUBE.Translate.z);
	meshList[GEO_CUBE]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_CUBE]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CUBE]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_CUBE]->material.kShininess = 1.f;


	meshList[GEO_TRACK] = MeshBuilder::GenerateOBJ("track", "OBJ//track.obj");
	meshList[GEO_TRACK]->textureID = LoadTGA("Image//track.tga");
	meshList[GEO_TRACK]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_TRACK]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TRACK]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TRACK]->material.kShininess = 1.f;
	TRACK.Scale = Vector3(4, 4, 4);
	TRACK.Translate = Vector3(0,-3, 0);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	
	bouncetime = fSpeed = fRotate = 0.f;
	switchlights = bCheckBrake = bTurningLeft = bTurningRight = false;

	InitLamppost();
	InitHeadlights();
	InitBrakelights();
}

bool TestDriveScene:: CheckCollision(object& one, object& two)
{
	if (
		(one.pos.x - one.size.x <= two.pos.x + two.size.x && one.pos.x + one.size.x >= two.pos.x - two.size.x) &&
		(one.pos.y - one.size.y <= two.pos.y + two.size.y && one.pos.y + one.size.y >= two.pos.y - two.size.y) &&
		(one.pos.z - one.size.z <= two.pos.z + two.size.z && one.pos.z + one.size.z >= two.pos.z - two.size.z)
		) {
		return true;
	}
	return false;
}

void TestDriveScene::Update(double dt)
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
	/*if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	if (Application::IsKeyPressed('5'))
	{
		//to do: switch light type to POINT and pass the information to
		light[0].type = Light::LIGHT_POINT;
	}
	else if (Application::IsKeyPressed('6'))
	{
		//to do: switch light type to DIRECTIONAL and pass the
		light[0].type = Light::LIGHT_DIRECTIONAL;
	}
	else if (Application::IsKeyPressed('7'))
	{
		//to do: switch light type to SPOT and pass the information to
		light[0].type = Light::LIGHT_SPOT;
	}*/
	
	//camera.Update(dt);
	camera.UpdateMovement(Vector3(player.pos.x, player.pos.y + 6, player.pos.z + 15), Vector3(player.pos.x, player.pos.y, player.pos.z));
	//camera.UpdateTurningMovement(bTurningLeft, bTurningRight, dt);
	//SetCursorPos(camera.setCursorX, camera.setCursorY);
	CalculateFrameRate();
	/*if (Application::IsKeyPressed('P'))
	{
		float currentTime = GetTickCount() * 0.001f;
		if (currentTime - bouncetime > 0.1f)
		{
			bouncetime = currentTime;
			switchlights = !switchlights;

			if (switchlights) {
				light[1].power = 1.f;
				glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
			}
			else
			{
				light[1].power = 0.f;
				glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
			}
		}
	}*/
	
	light[4].position.Set(movex + 2, movey, movez - 5);
	light[5].position.Set(movex - 1.3, movey, movez - 5);
	light[6].position.Set(movex + 0.2 , movey, movez + 2.5);

	DrivingMovement(dt);
	DayTimeNightTime();
	UpdateBrakelights();
	doCollision();
	//camera.mouse_callback();
}

void TestDriveScene::doCollision() {
	if (CheckCollision(player, cube)) {
		printf("YES\n");

	}else

	printf("NO\n");
}

/******************************************************************************/
/*!
\brief
	Alternate method to do TRS/render objects

\param	mesh
	mesh to render
\param	TRS
	class to set TRS (set in init func)
\param end
	check whether to pop model
\param	enableLight
	set Light as true/false
*/
/******************************************************************************/
void TestDriveScene::RenderOBJ(Mesh* mesh, TRS& trs, bool end, bool enableLight)
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

void TestDriveScene::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	RenderLamppost();
	RenderHeadlights();
	RenderBrakelights();
	RenderSkybox();
	RenderOBJ(meshList[GEO_TRACK], TRACK, true, true);

	//RenderOBJ(meshList[GEO_CUBE], CUBE, true, true);
	////Update the translate vector if theres is any transformation
	PLAYER.Translate = Vector3(movex, movey, movez);
	PLAYER.RotateY = Vector4(fRotate,0,1, 0);
	////Update the pos vector as well
	////if object is scaled, update the size vector
	player.pos = Vector3(PLAYER.Translate.x, PLAYER.Translate.y, PLAYER.Translate.z);
	RenderOBJ(meshList[GEO_CHAR], PLAYER, true, true);

	//No transform needed
	RenderTextOnScreen(meshList[GEO_TEXT], "W = forward", Color(0, 0, 0), 2, 0, 29);
	RenderTextOnScreen(meshList[GEO_TEXT], "S = backward", Color(0, 0, 0), 2, 0, 27);
	RenderTextOnScreen(meshList[GEO_TEXT], "A / D = steer", Color(0, 0, 0), 2, 0, 25);
	RenderTextOnScreen(meshList[GEO_TEXT], "SPACE = brake", Color(0, 0, 0), 2, 0, 23);
}

bool TestDriveScene::Change()
{
	return false;
}

void TestDriveScene::Exit()
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

void TestDriveScene::RenderMesh(Mesh* mesh, bool enableLight)
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

	if(mesh->textureID > 0){ 
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);} 
	else { 
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	} 
	mesh->Render(); //this line should only be called once in the whole function

	if(mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void TestDriveScene::RenderSkybox()
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
			modelStack.Rotate(-90.f, 0.f,0.f, 1.f);
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
		RenderMesh(meshList[GEO_BOTTOM], false);
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

void TestDriveScene::RenderText(Mesh* mesh, std::string text, Color color)
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

void TestDriveScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void TestDriveScene::CalculateFrameRate()
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

void TestDriveScene::InitLamppost()
{
	for (int i = 0; i < 4; i++)
	{
		light[i].type = Light::LIGHT_POINT;
		light[i].color.Set(0.5f, 0.5f, 0.5f);
		light[i].power = 1;
		light[i].kC = 1.f;
		light[i].kL = 0.01f;
		light[i].kQ = 0.001f;
		light[i].cosCutoff = cos(Math::DegreeToRadian(30));
		light[i].cosInner = cos(Math::DegreeToRadian(5));
		light[i].exponent = 3.f;
		light[i].spotDirection.Set(0.f, 1.f, 0.f);
		if (i < 2) // left
			light[i].position.Set(-20.f, 11.f, (15.f - (i * 30.f)));
		else // right
			light[i].position.Set(20.f, 11.f, (15.f - ((i - 2) * 30.f)));
	}
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
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

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
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

	meshList[GEO_LAMPPOST] = MeshBuilder::GenerateOBJ("lamppost", "OBJ//lamppost.obj");
	meshList[GEO_LAMPPOST]->textureID = LoadTGA("Image//lamppost.tga");
	meshList[GEO_LAMPPOST]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_LAMPPOST]->material.kDiffuse.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_LAMPPOST]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_LAMPPOST]->material.kShininess = 1.f;

	meshList[GEO_LIGHTCUBE] = MeshBuilder::GenerateCuboid("lightCube", Color(1.f, 1.f, 1.f), 1, 1, 1);
}

void TestDriveScene::RenderLamppost()
{
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

	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction
	else if (light[1].type == Light::LIGHT_SPOT)
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
	
	for (int i = 0; i < 4; i++)
	{
		if (i < 2) // left
		{
			modelStack.PushMatrix();
			///scale, translate, rotate 
				modelStack.Translate(-21.f, 0.f, (15.f - (i * 30.f)));
				modelStack.Scale(3.f, 3.f, 3.f);
				RenderMesh(meshList[GEO_LAMPPOST], true);
			modelStack.PopMatrix();
		}
		else // right
		{
			modelStack.PushMatrix();
			///scale, translate, rotate 
				modelStack.Translate(21.f, 0.f, (15.f - ((i- 2) * 30.f)));
				modelStack.Scale(3.f, 3.f, 3.f);
				modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
				RenderMesh(meshList[GEO_LAMPPOST], true);
			modelStack.PopMatrix();
		}
		modelStack.PushMatrix();
			modelStack.Translate(light[i].position.x, light[i].position.y, light[i].position.z);
			RenderMesh(meshList[GEO_LIGHTCUBE], false);
		modelStack.PopMatrix();
	}
}

void TestDriveScene::DayTimeNightTime()
{
	/*if (Application::IsKeyPressed('L'))
	{
		if (switchlights) // night
		{
			for (int i = 0; i < 4; i++)
			{
				light[i].power = 1.f;
				glUniform1f(m_parameters[U_LIGHT1_POWER], light[i].power);
			}
		}
		else // day
		{
			for (int i = 0; i < 4; i++)
			{
				light[i].power = 0.f;
				glUniform1f(m_parameters[U_LIGHT1_POWER], light[i].power);
			}
		}
	}*/
	
}

void TestDriveScene::InitHeadlights()
{
	for (int i = 4; i < 6; i++)
	{
		light[i].type = Light::LIGHT_SPOT;
		light[i].color.Set(0.5f, 0.5f, 0.5f);
		light[i].power = 1;
		light[i].kC = 1.f;
		light[i].kL = 0.01f;
		light[i].kQ = 0.001f;
		light[i].cosCutoff = cos(Math::DegreeToRadian(20));
		light[i].cosInner = cos(Math::DegreeToRadian(5));
		light[i].exponent = 3.f;
		light[i].spotDirection.Set(0.f, 0.5f, 1.f);
		if (i == 4) // left
			light[i].position.Set(movex +2.5, movey, movez-5);
		else // right
			light[i].position.Set(movex -2.5, movey, movez-5);
	}
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

	meshList[GEO_HEADLIGHTS] = MeshBuilder::GenerateOBJ("headlights1", "OBJ//headlights1.obj");
}

void TestDriveScene::RenderHeadlights()
{
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
	
	modelStack.PushMatrix();
		modelStack.Translate(light[4].position.x, light[4].position.y, light[4].position.z);
		RenderMesh(meshList[GEO_HEADLIGHTS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
		modelStack.Translate(light[5].position.x, light[5].position.y, light[5].position.z);
		RenderMesh(meshList[GEO_HEADLIGHTS], false);
	modelStack.PopMatrix();

}

void TestDriveScene::InitBrakelights()
{
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
	light[6].type = Light::LIGHT_POINT;
	light[6].color.Set(0.5f, 0.f, 0.f);
	light[6].power = 0.5;
	light[6].kC = 1.f;
	light[6].kL = 0.01f;
	light[6].kQ = 0.001f;
	light[6].cosCutoff = cos(Math::DegreeToRadian(20));
	light[6].cosInner = cos(Math::DegreeToRadian(5));
	light[6].exponent = 3.f;
	light[6].spotDirection.Set(0.f, 0.5f, 1.f);
	light[6].position.Set(movex + 0.2, movey, movez + 2.5);
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

	meshList[GEO_BRAKELIGHTS] = MeshBuilder::GenerateCuboid("lightCube", Color(1.f, 0.f, 0.f), 2, 0.3, 0.3);
}

void TestDriveScene::RenderBrakelights()
{
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

	modelStack.PushMatrix();
		modelStack.Translate(light[6].position.x, light[6].position.y, light[6].position.z);
		RenderMesh(meshList[GEO_BRAKELIGHTS], false);
	modelStack.PopMatrix();
}

void TestDriveScene::UpdateBrakelights()
{
	if (bCheckBrake) 
	{
		light[6].power = 0.5f;
		glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
	}
	else 
	{
		light[6].power = 0.f;
		glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);	
	}
}

void TestDriveScene::DrivingMovement(double dt)
{
	//std::cout << dt;
	//std::cout << fSpeed;
	std::cout << fRotate;

	// max speed
	if (fSpeed > 0.3)
		fSpeed = 0.3;

	if (Application::IsKeyPressed('W') && !(Application::IsKeyPressed(VK_SPACE))) 
	{	// forward
		if (Application::IsKeyPressed('A'))
		{	// steer left while moving forward
			bTurningLeft = true;
			fRotate += 0.2;
			movex -= fSpeed / 3.0;
			movez -= fSpeed / 3.0;
		}
		if (Application::IsKeyPressed('D'))
		{	// steer right while moving forward
			bTurningRight = true;
			fRotate -= 0.2;
			movex += fSpeed / 3.0;
			movez -= fSpeed / 3.0;
		}

		if (fRotate > 0)
		{
			movex -= fSpeed / 3.0;
			movez -= fSpeed / 3.0;
		}
		else if (fRotate < 0)
		{
			movex += fSpeed / 3.0;
			movez -= fSpeed / 3.0;
		}
		else
		{
			movez -= fSpeed;
		}

		fSpeed += dt / 3.0;
	}

	// slide only works with forward
	/*if (fSpeed > 0.0 && !(Application::IsKeyPressed('S')) && !(Application::IsKeyPressed('W')))
	{
		movez -= fSpeed;
		fSpeed -= dt / 3.0;
	}
	else if (fSpeed < 0.0)
		fSpeed = 0;
	
	if (Application::IsKeyPressed('A') && fSpeed != 0)
	{
		bTurningLeft = true;
		
		fRotate += 2;
		movex -= fSpeed / 3.0;
		movez -= fSpeed / 3.0;
	}
	if (Application::IsKeyPressed('D') && fSpeed != 0)
	{
		bTurningRight = true;
		fRotate -= 2;
		movex += fSpeed / 3.0;
		movez -= fSpeed / 3.0;
	}*/
	

	if (Application::IsKeyPressed('S') && !(Application::IsKeyPressed(VK_SPACE)))
	{	// backward
		if (Application::IsKeyPressed('A'))
		{	// steer left while moving backward
			bTurningLeft = true;
			fRotate -= 0.2;
			movex -= fSpeed / 3.0;
			movez += fSpeed / 3.0;
		}
		if (Application::IsKeyPressed('D'))
		{	// steer right while moving backward
			bTurningRight = true;
			fRotate += 0.2;
			movex += fSpeed / 3.0;
			movez += fSpeed / 3.0;
		}

		if (fRotate > 0)
		{
			movex += fSpeed / 3.0;
			movez += fSpeed / 3.0;
		}
		else if (fRotate < 0)
		{
			movex -= fSpeed / 3.0;
			movez += fSpeed / 3.0;
		}
		else
		{
			movez += fSpeed;
		}

		fSpeed += dt / 3.0;
	}
	

	if (Application::IsKeyPressed(VK_SPACE)) //brake
	{
		bCheckBrake = true;
		
		if (fSpeed > 0.0)
			fSpeed -= dt / 2.0;
	}
	else
		bCheckBrake = false;
}
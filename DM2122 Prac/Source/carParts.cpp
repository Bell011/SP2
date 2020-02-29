#include "carParts.h"
#include "GL\glew.h"

#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

Car::Car(Mesh* w, Mesh* f)
{
}

Mesh* Car::getWheel()
{
	return nullptr;
}

Mesh* Car::getCarframe()
{
	return nullptr;
}

void Car::setWheel(Mesh* w)
{
}

void Car::setCarFrame(Mesh* f)
{
}

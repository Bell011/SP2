#include "carParts.h"
#include "GL\glew.h"

#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

Car::Car(Mesh* w = nullptr, Mesh* f = nullptr)
{
	swheel = w;
	scarframe = f;
}

Mesh* Car::getWheel()
{
	return swheel;
}

Mesh* Car::getCarframe()
{
	return scarframe;
}

void Car::setWheel(Mesh* w)
{
	swheel = w;
}

void Car::setCarFrame(Mesh* f)
{
	scarframe = f;
}

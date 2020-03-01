#include "carParts.h"
#include "GL\glew.h"

#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

Car::Car() {
	car = nullptr;
}
Car::Car(Mesh* c)
{
	car = c;
}

Mesh* Car::getCar()
{
	return car;
}

void Car::setCar(Mesh* c)
{
	car = c;
}

Mesh* Car::getWheel()
{
	return wheel;
}

void Car::setWheel(Mesh* w)
{
	wheel = w;
}

void Car::setWheelPos(int n , Vector3 p)
{
	wheelpos[n] = p;
}

Vector3 Car::getWheelPos(int n)
{
	return wheelpos[n];
}

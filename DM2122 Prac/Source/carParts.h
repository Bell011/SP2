#ifndef CARPARTS_H
#define CARPARTS_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Mesh.h"

class Car
{
private:
	Mesh* car;
	Mesh* wheel;
	Vector3 wheelpos[4];
	Vector3 front;

public:
	Car();
	Vector3 getFront();
	void setFront(Vector3);
	Mesh* getCar();
	void setCar(Mesh*);
	Mesh* getWheel();
	void setWheel(Mesh*);
	void setWheelPos(int, Vector3);
	Vector3 getWheelPos(int);
};


#endif
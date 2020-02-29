#ifndef CARPARTS_H
#define CARPARTS_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"
#include "camerafps.h"
#include <iostream>
#include <string>
#include "collision.h"
enum car_wheel {
	WHEEL1,
	WHEEL2,
	WHEEL3,
};
enum carframe {
	cf1,
	cf2,
	cf3,
};
enum car_texture {
	// 16 combi here (refer to text file)
};
class Car
{
private:

	Mesh* swheel;
	Mesh* scarframe;

	

public:
	Car(Mesh* w, Mesh* f);
	Mesh* getWheel();
	Mesh* getCarframe();
	void setWheel(Mesh* w);
	void setCarFrame(Mesh* f);

};


#endif
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
	NUM_WHEEL
};
enum carframe {
	cf1,
	cf2,
	cf3,
	NUM_CAR
};
enum car_texture {
	c1k1,
	c1k2,
	c1b1,
	c1b2,
	c1r1,
	c1r2,
	c1w1,
	c1w2,

	c2k1,
	c2k2,
	c2b1,
	c2b2,
	c2r1,
	c2r2,
	c2w1,
	c2w2,

	c3k1,
	c3k2,
	c3b1,
	c3b2,
	c3r1,
	c3r2,
	c3w1,
	c3w2,
	NUM_TEXTURE
};
class Car
{
private:
	Mesh* swheel[NUM_WHEEL];
	Mesh* scarframe[NUM_CAR];
public:
	Car(Mesh* w, Mesh* f);
	Mesh* getWheel();
	Mesh* getCarframe();
	static void setWheel(Mesh* w);
	static void setCarFrame(Mesh* f);
};


#endif
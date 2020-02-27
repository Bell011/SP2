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

class carParts
{
private:
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
	
	car_wheel wh;
	carframe cf;
	car_texture txt;

public:
	carParts(car_texture t, car_wheel w, carframe c);
	car_wheel getCarWheel();
	carframe getCarframe();
	car_texture getCarTexture();

};


#endif
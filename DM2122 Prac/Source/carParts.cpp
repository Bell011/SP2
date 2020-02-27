#include "carParts.h"
#include "GL\glew.h"

#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

carParts::carParts(car_texture t, car_wheel w, carframe c)
{
	txt = t;
	wh = w;
	cf = c;
}

carParts::car_wheel carParts::getCarWheel()
{
	return wh;
}

carParts::carframe carParts::getCarframe()
{
	return cf;
}

carParts::car_texture carParts::getCarTexture()
{
	return txt;
}
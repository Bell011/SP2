#include "CarEntity.h"

entityCar::entityCar()
{

}

entityCar::~entityCar()
{

}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SETS

*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void entityCar::setPos(float x, float y, float z)
{
	this->carPos.x = x;
	this->carPos.y = y;
	this->carPos.z = z;
}

void entityCar::setVel(float x, float y, float z)
{
	this->carVel.x = x;
	this->carVel.y = y;
	this->carVel.z = z;
}

void entityCar::setSize(float x, float y, float z)
{
	this->carSize.x = x;
	this->carSize.y = y;
	this->carSize.z = z;
}

void entityCar::setScore(float score)
{
	this->scoreRace = score;
}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GETS

*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 entityCar::getPos()
{
	return this->carPos;
}

Vector3 entityCar::getVel()
{
	return this->carVel;
}

Vector3 entityCar::getSize()
{
	return this->carSize;
}

float entityCar::getScore()
{
	return this->scoreRace;
}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FUNCTIONS

*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool entityCar::isCollide(entityCar* enemy, entityCar* player)
{	
	if (enemy->carPos.x * enemy->carSize.x >= player->carPos.x * player->carSize.x && enemy->carPos.z * enemy->carSize.z == player->carPos.z * carSize.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void entityCar::updatePos()
{
	
	carPos.z += carVel.z;
}

void entityCar::updateLives(int count)
{

}
#ifndef _CAR_ENTITY_H
#define _CAR_ENTITY_H

#include "Utility.h"


class entityCar
{
private:
	entityCar();
	~entityCar();
	Vector3 carPos;
	Vector3 carSize;
	Vector3 carVel;
	
	float scoreRace;
public:
	void setPos(float x, float y, float z);
	void setVel(float x, float y, float z);
	void setSize(float x, float y, float z);
	void setScore(float score);
	Vector3 getPos();
	Vector3 getVel();
	Vector3 getSize();
	float getScore();

	bool isCollide(entityCar* enemy, entityCar* player);


};

#endif _CAR_ENTITY_H
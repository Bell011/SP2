#ifndef _CAR_ENTITY_H
#define _CAR_ENTITY_H

#include "Utility.h"
#include "Application.h"


class entityCar
{
private:
	Vector3 carPos;
	Vector3 carSize;
	Vector3 carVel;

	int lives;
	float scoreRace;
public:
	entityCar();
	~entityCar();
	void setPos(float x, float y, float z);
	void setVel(float x, float y, float z);
	void setSize(float x, float y, float z);
	void setScore(float score);
	void setLives(float lives);
	Vector3 getPos();
	Vector3 getVel();
	Vector3 getSize();
	float getScore();
	float getLives();

	void updateLives(int count);

	bool isCollide(entityCar* enemy, entityCar* player);
	static void updatePos(entityCar* enemy1, entityCar* enemy2, entityCar* enemy3, entityCar* player,double dt);

};

#endif _CAR_ENTITY_H
#ifndef _G3_CAR_ENTITY_H
#define _G3_CAR_ENTITY_H

#include "Utility.h"
#include "Application.h"
#include "collcorners.h"

class g3CarEntity
{
private:
	Vector3 carPos;
	Vector3 carSize;
	Vector3 carVel;

	int lives;
	float scoreRace;
public:
	g3CarEntity();
	~g3CarEntity();
	void setPos(float x, float y, float z);
	void setVel(float x, float y, float z);
	void setSize(corners&);
	void setScore(float score);
	void setLives(int lives);
	Vector3 getPos();
	Vector3 getVel();
	Vector3 getSize();
	float getScore();
	int getLives();

	void updateLives(int count);

	bool isCollide(g3CarEntity* enemy, g3CarEntity* player);
	static void updatePos(g3CarEntity* enemy1, g3CarEntity* enemy2, g3CarEntity* enemy3, g3CarEntity* player,double dt);

};

#endif _CAR_ENTITY_H
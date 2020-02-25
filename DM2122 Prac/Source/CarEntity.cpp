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
	carPos.x = x;
	carPos.y = y;
	carPos.z = z;
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

void entityCar::setLives(float lives)
{
	this->lives = lives;
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

float entityCar::getLives()
{
	return this->lives;
}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FUNCTIONS

*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool entityCar::isCollide(entityCar* enemy, entityCar* player)
{
	if ((enemy->carPos.x - enemy->carSize.x <= player->carPos.x + player->carPos.x && enemy->carPos.x + enemy->carSize.x >= player->carPos.x - player->carSize.x) &&
		(enemy->carPos.z - enemy->carSize.z <= player->carPos.z + player->carSize.z && enemy->carPos.z + enemy->carSize.z >= player->carPos.z - player->carSize.z))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void entityCar::updatePos(entityCar* enemy1, entityCar* enemy2, entityCar* enemy3, entityCar* player, double dt)
{
	
	enemy1->carPos += enemy1->carVel * dt;
	enemy2->carPos += enemy2->carVel * dt;
	enemy3->carPos += enemy3->carVel * dt;


	if (enemy1->carPos.z <= 0.0f)
	{
		enemy1->carPos.z = 50.0f;
	}
	if (enemy2->carPos.z <= 0.0f)
	{
		enemy2->carPos.z = 55.0f;
	}
	if (enemy3->carPos.z <= 0.0f)
	{
		enemy3->carPos.z = 65.0f;
	}
	if (player->carPos.x <= 10.f)
	{
		if (Application::IsKeyPressed(VK_LEFT))
		{
			player->carPos.x += 5.f * dt;
		}

	}
	if (player->carPos.x >= 0.f)
	{
		if (Application::IsKeyPressed(VK_RIGHT))
		{
			player->carPos.x -= 5.f * dt;
		}
	}
	if (player->carPos.x >= 10.0f)
	{
		player->carPos.x == 9.0f;
		player->updateLives(-1); 
	}
	else if (player->carPos.x <= 0.f)
	{
		player->carPos.x = 1.f;
		player->updateLives(-1);
	}

}

void entityCar::updateLives(int count)
{
	lives += count;
}
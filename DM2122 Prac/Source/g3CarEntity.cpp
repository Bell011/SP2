#include "g3CarEntity.h"

g3CarEntity::g3CarEntity()
{

}

g3CarEntity::~g3CarEntity()
{

}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SETS

*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void g3CarEntity::setPos(float x, float y, float z)
{
	carPos.x = x;
	carPos.y = y;
	carPos.z = z;
}

void g3CarEntity::setVel(float x, float y, float z)
{
	this->carVel.x = x;
	this->carVel.y = y;
	this->carVel.z = z;
}

void g3CarEntity::setSize(corners& c)
{
	
	this->carSize.x = (c.getMax().x - c.getMin().x) / 2;
	this->carSize.y = (c.getMax().y - c.getMin().y) / 2;
	this->carSize.z = (c.getMax().z - c.getMin().z) / 2;
}

void g3CarEntity::setScore(float score)
{
	this->scoreRace = score;
}

void g3CarEntity::setLives(int lives)
{
	this->lives = lives;
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GETS

*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 g3CarEntity::getPos()
{
	return this->carPos;
}

Vector3 g3CarEntity::getVel()
{
	return this->carVel;
}

Vector3 g3CarEntity::getSize()
{
	return this->carSize;
}

float g3CarEntity::getScore()
{
	return this->scoreRace;
}

int g3CarEntity::getLives()
{
	return this->lives;
}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FUNCTIONS

*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool g3CarEntity::isCollide(g3CarEntity* enemy, g3CarEntity* player)
{
	if ((enemy->carPos.x - enemy->carSize.x <= player->carPos.x + player->carPos.x && enemy->carPos.x + enemy->carSize.x >= player->carPos.x - player->carSize.x) &&
		(enemy->carPos.z - enemy->carSize.z <= player->carPos.z + player->carSize.z && enemy->carPos.z + enemy->carSize.z >= player->carPos.z - player->carSize.z))
	{
		return true;
	}
		return false;
	
}
void g3CarEntity::updatePos(g3CarEntity* enemy1, g3CarEntity* enemy2, g3CarEntity* enemy3, g3CarEntity* player, double dt)
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
		player->carPos.x = 9.0f;
		player->updateLives(-1); 
	}
	else if (player->carPos.x <= 0.f)
	{
		player->carPos.x = 1.f;
		player->updateLives(-1);
	}

}

void g3CarEntity::updateLives(int count)
{
	lives += count;
}
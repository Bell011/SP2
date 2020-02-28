#ifndef COLLISION_H
#define COLLISION_H
#include "Vector3.h"
struct object {
	Vector3 front;
	Vector3 size;
	Vector3 pos;
	object(Vector3 size = Vector3(1,1,1), Vector3 pos = Vector3(0,0,0)){ 
		this->size = size;
		this->pos = pos;
	}
};

class Collision {
private:
	Vector3 front;
	Vector3 right;
	Vector3 up;

public:
	Collision(Vector3, Vector3, Vector3);
	~Collision();
	Vector3 getFront();
	Vector3 getRight();
	Vector3 getUp();
	

};


#endif
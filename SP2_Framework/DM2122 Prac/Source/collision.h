#ifndef COLLISION_H
#define COLLISION_H
#include "Vector3.h"
struct object {
	Vector3 size;
	Vector3 pos;
	object(Vector3 size = Vector3(1, 1, 1), Vector3 pos = Vector3(0, 0, 0)) {
		this->size = size;
		this->pos = pos;
	}
};


#endif
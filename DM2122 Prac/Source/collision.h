#ifndef COLLISION_H
#define COLLISION_H
#include "Vector3.h"
class AABBobj {
	private:
	Vector3 size;
	Vector3 pos;
	public:
	AABBobj(Vector3 size = Vector3(1,1,1), Vector3 pos = Vector3(0,0,0)){ 
		this->size = size;
		this->pos = pos;
	}
	void setSize(Vector3 s) {
		size = s;
	}
	void setPos(Vector3 p) {
		pos = p;
	}
	Vector3 getPos() {
		return pos;
	}
	Vector3 getSize() {
		return size;
	}
};


#endif
#pragma once
#include "Object.h"
#include "rectObject.h"
#include <MyMath.h>
class cirObject : public Object
{
private:
	float radius;
public:
	cirObject(Vector3 pos = Vector3(0,0,0), float r=1.f) :Object(pos) {
		radius = r;

	}
	~cirObject() {

	}
	float getRadius() {
		return radius;
	}
	void setRadius(float r) {
		radius = r;
	}
	bool checkCollision(Object* obj) {
		float closestx = Math::Max(obj->getPos().x - ((rectObj*)obj)->getSize().x, Math::Min(getPos().x, obj->getPos().x + ((rectObj*)obj)->getSize().x));
		float closesty = Math::Max(obj->getPos().y - ((rectObj*)obj)->getSize().y, Math::Min(getPos().y, obj->getPos().y + ((rectObj*)obj)->getSize().y));
		float closestz = Math::Max(obj->getPos().z - ((rectObj*)obj)->getSize().z, Math::Min(getPos().z, obj->getPos().z + ((rectObj*)obj)->getSize().z));

		float distance = Math::Square(closestx - getPos().x) + Math::Square(closesty - getPos().y) + Math::Square(closestz - getPos().z);

		if (distance < Math::Square(getRadius())) {
			return true;
		}
	
			return false;
	
	}

};



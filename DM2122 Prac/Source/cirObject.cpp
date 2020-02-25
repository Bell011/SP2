#include "cirObject.h"
cirObject::cirObject(Vector3 pos, float r): Object (pos) {
	OBJECT_TYPE type = CIRC;
	radius = r;
}
cirObject::~cirObject(){

}
float cirObject::getRadius() {
	return radius;
}
void cirObject::setRadius(float r) {
	radius = r;
}
bool cirObject::CheckCollision(Object*obj)
{
	if (obj->getType() == CIRC) {
		if ((getPosition() - obj->getPosition()).LengthSquared() == (radius + ((cirObject*)obj)->getRadius()) * (radius + ((cirObject*)obj)->getRadius())) {
			return true;
		}
	}
	else if (obj->getType() == REC) {
		if (fabsf((((getPosition() + (((getPosition() - obj->getPosition()).Normalized())*radius)) - obj->getPosition()).Dot(((rectObject*)obj)->getFront().Normalized()))) <= ((rectObject*)obj)->getFront().Length()
			&& fabsf((((getPosition() + (((getPosition() - obj->getPosition()).Normalized()) * radius)) - obj->getPosition()).Dot(((rectObject*)obj)->getRight().Normalized()))) <= ((rectObject*)obj)->getRight().Length()
			&& fabsf((((getPosition() + (((getPosition() - obj->getPosition()).Normalized()) * radius)) - obj->getPosition()).Dot(((rectObject*)obj)->getUp().Normalized()))) <= ((rectObject*)obj)->getUp().Length()){
			return true;
		}
	}
	return false;

}

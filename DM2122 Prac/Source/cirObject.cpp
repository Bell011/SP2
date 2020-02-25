#include "cirObject.h"
cirObject::cirObject(Vector3 pos, TRS trs, float r): Object (pos,trs) {
	setType(CIRC);
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
bool cirObject::CheckCollision(Object*obj, TRS& trs1,TRS& trs2)
{
	setPosition(Vector3(trs1.Translate.x, trs1.Translate.y, trs1.Translate.z));
	obj->setPosition(Vector3(trs2.Translate.x, trs2.Translate.y, trs2.Translate.z));
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

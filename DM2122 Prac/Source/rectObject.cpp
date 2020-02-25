#include "rectObject.h"

rectObject::rectObject(Vector3 pos, TRS trs, Vector3 f, Vector3 r, Vector3 u) : Object(pos,trs){
	setType(REC);
	front = f;
	right = r;
	up = u;
}
rectObject::~rectObject() {

}
Vector3 rectObject::getFront() {
	return front;
}
Vector3 rectObject::getRight() {
	return right;
}
Vector3 rectObject::getUp() {
	return up;
}
void rectObject::setFront(Vector3 f) {
	front = f;
}
void rectObject::setRight(Vector3 r) {
	right = r;
}
void rectObject::setUp(Vector3 u) {
	up = u;
}
bool rectObject::CheckCollision(Object*obj,TRS& trs1, TRS& trs2)
{
	setPosition(Vector3(trs1.Translate.x, trs1.Translate.y, trs1.Translate.z));
	obj->setPosition(Vector3(trs2.Translate.x, trs2.Translate.y, trs2.Translate.z));
	if (obj->getType() == REC) {
		Vector3 corners[8] = {
			(getPosition() + getFront() + getRight() + getUp()),
			(getPosition() + getFront() - getRight() + getUp()),
			(getPosition() + getFront() + getRight() - getUp()),
			(getPosition() + getFront() - getRight() - getUp()),
			(getPosition() - getFront() + getRight() + getUp()),
			(getPosition() - getFront() - getRight() + getUp()),
			(getPosition() - getFront() + getRight() - getUp()),
			(getPosition() - getFront() - getRight() - getUp())
		};

		Vector3 objcorners[8] = {
		(obj->getPosition() + ((rectObject*)obj)->getFront() + ((rectObject*)obj)->getRight() + ((rectObject*)obj)->getUp()),
		(obj->getPosition() + ((rectObject*)obj)->getFront() - ((rectObject*)obj)->getRight() + ((rectObject*)obj)->getUp()),
		(obj->getPosition() + ((rectObject*)obj)->getFront() + ((rectObject*)obj)->getRight() - ((rectObject*)obj)->getUp()),
		(obj->getPosition() + ((rectObject*)obj)->getFront() - ((rectObject*)obj)->getRight() - ((rectObject*)obj)->getUp()),
		(obj->getPosition() - ((rectObject*)obj)->getFront() + ((rectObject*)obj)->getRight() + ((rectObject*)obj)->getUp()),
		(obj->getPosition() - ((rectObject*)obj)->getFront() - ((rectObject*)obj)->getRight() + ((rectObject*)obj)->getUp()),
		(obj->getPosition() - ((rectObject*)obj)->getFront() + ((rectObject*)obj)->getRight() - ((rectObject*)obj)->getUp()),
		(obj->getPosition() - ((rectObject*)obj)->getFront() - ((rectObject*)obj)->getRight() - ((rectObject*)obj)->getUp())
		};
		for (int i = 0; i < 8; ++i) {
			if (fabsf((corners[i] - obj->getPosition()).Dot((((rectObject*)obj)->getFront()).Normalized())) <= (((rectObject*)obj)->getFront()).Length() && fabsf((corners[i] - obj->getPosition()).Dot((((rectObject*)obj)->getFront()).Normalized())) <= (((rectObject*)obj)->getRight()).Length() && fabsf((corners[i] - obj->getPosition()).Dot((((rectObject*)obj)->getUp()).Normalized())) <= (((rectObject*)obj)->getUp()).Length()
				|| fabsf((objcorners[i] - getPosition()).Dot(front.Normalized())) <= front.Length() && fabsf((objcorners[i] - getPosition()).Dot(right.Normalized())) <= right.Length() && fabsf((objcorners[i] - getPosition()).Dot(up.Normalized())) <= up.Length()) {
				return true;
			}
		}
	}
	else if (obj->getType() == CIRC) {
		if (fabsf((((obj->getPosition() + (((getPosition() - obj->getPosition()).Normalized()) * ((cirObject*)obj)->getRadius())) - obj->getPosition()).Dot(getFront().Normalized()))) <= ((rectObject*)obj)->getFront().Length()
			&& fabsf((((obj->getPosition() + (((getPosition() - obj->getPosition()).Normalized()) * ((cirObject*)obj)->getRadius())) - obj->getPosition()).Dot(getRight().Normalized()))) <= ((rectObject*)obj)->getRight().Length()
			&& fabsf((((obj->getPosition() + (((getPosition() - obj->getPosition()).Normalized()) * ((cirObject*)obj)->getRadius())) - obj->getPosition()).Dot(getUp().Normalized()))) <= ((rectObject*)obj)->getUp().Length()) {
			return true;
		}
	}
		
	
	return false;
}

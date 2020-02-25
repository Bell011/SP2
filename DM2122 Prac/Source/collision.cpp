#include "collision.h"

Collision::Collision(Vector3 f, Vector3 r, Vector3 u) {
	front = f;
	right = r;
	up = u;
}
Collision::~Collision() {

}
Vector3 Collision::getFront() {
	return front;
}
Vector3 Collision::getRight() {
	return right;
}
Vector3 Collision::getUp() {
	return up;
}
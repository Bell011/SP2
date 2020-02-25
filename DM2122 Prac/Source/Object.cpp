#include "Object.h"
Object::Object(Vector3 pos, TRS trs) {
	position = pos;
	transform = trs;
}
Object::~Object() {

}
Vector3 Object::getPosition(){
	return position;
}
OBJECT_TYPE Object::getType() {
	return type;
}
TRS Object::getTRS() {
	return transform;
}
void Object::setPosition(Vector3 pos) {
	position =  pos;
}
void Object::setType(OBJECT_TYPE ty) {
	type = ty;
}
void Object::setTRS(TRS& trs) {
	transform = trs;
}
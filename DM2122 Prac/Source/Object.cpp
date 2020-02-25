#include "Object.h"
Object::Object(Vector3 pos) {
	position = pos;
}
Object::~Object() {

}
Vector3 Object::getPosition(){
	return position;
}
OBJECT_TYPE Object::getType() {
	return type;
}
void Object::setPosition(Vector3 pos) {
	position =  pos;
}
void Object::setType(OBJECT_TYPE ty) {
	type = ty;
}
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
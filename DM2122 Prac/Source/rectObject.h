#ifndef RECT_OBJECT_H
#define RECT_OBJECT_H
#include "Object.h"
#include "cirObject.h"

class rectObject: public Object {
private:
	Vector3 front;
	Vector3 right;
	Vector3 up;

public:
	rectObject(Vector3, Vector3, Vector3, Vector3);
	~rectObject();
	Vector3 getFront();
	Vector3 getRight();
	Vector3 getUp();
	bool CheckCollision(Object*);


};
#endif

#pragma once
#include "Object.h"
#include "cirObject.h"
class rectObject : public Object {
private:
	Vector3 front;
	Vector3 right;
	Vector3 up;

public:
	rectObject(Vector3 pos = Vector3(0.f, 0.f, 0.f), Vector3 f = Vector3(0.f, 0.f, 1.f), Vector3 r = Vector3(1.f, 0.f, 0.f), Vector3 u = Vector3(0.f, 1.f, 0.f));
	~rectObject();
	Vector3 getFront();
	Vector3 getRight();
	Vector3 getUp();
	void setFront(Vector3);
	void setRight(Vector3);
	void setUp(Vector3);
	bool CheckCollision(Object*);


};

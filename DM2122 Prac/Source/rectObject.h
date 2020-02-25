#pragma once
#include "Object.h"
#include "cirObject.h"
class rectObject : public Object {
private:
	Vector3 front;
	Vector3 right;
	Vector3 up;

public:
	rectObject(Vector3 pos, TRS trs ,Vector3 f, Vector3 r, Vector3 u);
	~rectObject();
	Vector3 getFront();
	Vector3 getRight();
	Vector3 getUp();
	void setFront(Vector3);
	void setRight(Vector3);
	void setUp(Vector3);
	bool CheckCollision(Object*,TRS&,TRS&);


};

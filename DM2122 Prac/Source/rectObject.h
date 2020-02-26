#pragma once
#include "Object.h"
#include "cirObject.h"
//class rectObject : public Object {
//private:
//	Vector3 front;
//	Vector3 right;
//	Vector3 up;
//
//public:
//	rectObject(Vector3 pos, TRS trs ,Vector3 f, Vector3 r, Vector3 u);
//	~rectObject();
//	Vector3 getFront();
//	Vector3 getRight();
//	Vector3 getUp();
//	void setFront(Vector3);
//	void setRight(Vector3);
//	void setUp(Vector3);
//	bool CheckCollision(Object*,TRS&,TRS&);
//};
class rectObj:public Object {
private:
	Vector3 size;
public:
	rectObj(Vector3 s = Vector3(1, 1, 1), Vector3 p = Vector3(0, 0, 0)):Object(p) {
		size = s;
	}
	~rectObj() {

	}
	void setSize(corners& c) {
		size = Vector3(c.getMax().x - c.getMin().x, c.getMax().y - c.getMin().y, c.getMax().z - c.getMin().z);
	}
	void setManualSize(Vector3 s) {
		size = s;
	}
	Vector3 getSize() {
		return size;
	}
	bool checkCollision(Object*two) { //against rectangle obj
		if (
			(getPos().x - getSize().x <= two->getPos().x + ((rectObj*)two)->getSize().x && getPos().x + getSize().x >= two->getPos().x - ((rectObj*)two)->getSize().x) &&
			(getPos().y - getSize().y <= two->getPos().y + ((rectObj*)two)->getSize().y && getPos().y + getSize().y >= two->getPos().y - ((rectObj*)two)->getSize().y) &&
			(getPos().z - getSize().z <= two->getPos().z + ((rectObj*)two)->getSize().z && getPos().z + getSize().z >= two->getPos().z - ((rectObj*)two)->getSize().z)
			) {
			return true;
		}
		return false;
	}
};

#ifndef OBJECT_H
#define OBJECT_H
#include <Vector3.h>
#include <MatrixStack.h>
#include"collcorners.h"
enum OBJECT_TYPE {
	REC,
	CIRC
};
//class Object
//{
//private:
//	Vector3 position;
//	OBJECT_TYPE type;
//	TRS transform;
//public:
//	Object(Vector3, TRS&);
//	~Object();
//	Vector3 getPosition();
//	void setPosition(Vector3);
//	OBJECT_TYPE getType();
//	virtual bool CheckCollision(Object*)=0;
//};
class Object {
private:
	Vector3 pos;
public:
	Object(Vector3 pos = Vector3(0, 0, 0)) {
		this->pos = pos;
	}
	~Object() {

	}
	void setPos(Vector3 p) {
		pos = p;
	}
	Vector3 getPos() {
		return pos;
	}
	virtual bool checkCollision(Object*) = 0;
};
#endif


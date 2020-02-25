#ifndef OBJECT_H
#define OBJECT_H
#include <Vector3.h>
enum OBJECT_TYPE {
	REC,
	CIRC
};
class Object
{
private:
	Vector3 position;
	OBJECT_TYPE type;
public:
	Object(Vector3);
	~Object();
	Vector3 getPosition();
	void setPosition(Vector3);
	OBJECT_TYPE getType();
	virtual bool CheckCollision(Object*)=0;
};
#endif


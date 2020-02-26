#ifndef CIR_OBJECT_H
#define CIR_OBJECT_H
#include "Object.h"
#include "rectObject.h"
class cirObject : public Object
{
private:
	float radius;
public:
	cirObject(Vector3 pos = Vector3(0.f, 0.f, 0.f), float r = 0.f);
	~cirObject();
	float getRadius();
	bool CheckCollision(Object*);
};
#endif


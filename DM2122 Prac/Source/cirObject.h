#ifndef CIR_OBJECT_H
#define CIR_OBJECT_H
#include "Object.h"
#include "rectObject.h"
class cirObject : public Object
{
private:
	float radius;
public:
	cirObject(Vector3 pos, TRS trs, float r);
	~cirObject();
	float getRadius();
	void setRadius(float);
	bool CheckCollision(Object* obj, TRS& trs1, TRS& trs2);
};
#endif


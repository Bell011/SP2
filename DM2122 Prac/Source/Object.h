#ifndef OBJECT_H
#define OBJECT_H
#include <Vector3.h>
#include <MatrixStack.h>
enum OBJECT_TYPE {
	REC,
	CIRC
};
class Object
{
private:
	Vector3 position;
	OBJECT_TYPE type;
	TRS transform;
public:
	Object(Vector3, TRS);
	~Object();
	Vector3 getPosition();
	void setPosition(Vector3);
	OBJECT_TYPE getType();
	void setType(OBJECT_TYPE);
	TRS getTRS();
	void setTRS(TRS&);
	virtual bool CheckCollision(Object*,TRS& trs1,TRS&trs2)=0;
};
#endif


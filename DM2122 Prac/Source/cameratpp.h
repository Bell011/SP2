#ifndef CAMERA_TPP_H
#define CAMERA_TPP_H
#include "Camera.h"

class cameratpp : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;
	Vector3 view;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	int setCursorX;
	int setCursorY;

	cameratpp();
	~cameratpp();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
	virtual void mouse_callback();
};

#endif
#ifndef CAMERA_FPS_H
#define CAMERA_FPS_H
#include "Camera.h"

class camerafps : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Vector3 view;
	Vector3 right;

	float yaw, pitch, fov, lastX, lastY;
	bool firstMouse;

	camerafps();
	~camerafps();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
	virtual void mouse_callback();
};
#endif
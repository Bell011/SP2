#include "cameratpp.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"

cameratpp::cameratpp()
{
}

cameratpp::~cameratpp()
{
}

void cameratpp::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	view = target - position;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	setCursorX = 960;
	setCursorY = 540;
	SetCursorPos(setCursorX, setCursorY);
}

void cameratpp::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;
	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}

	if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;
	}
	
	mouse_callback();
	position = target - view;
}

void cameratpp::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void cameratpp::mouse_callback()
{
	Vector3 right = view.Cross(up).Normalized();
	POINT mousePos;
	GetCursorPos(&mousePos);
	int xpos = mousePos.x;
	int ypos = mousePos.y;

	float xoffset = setCursorX - xpos;
	float yoffset = setCursorY - ypos;

	float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Mtx44 yaw;
	Mtx44 pitch;
	Vector3 tempview = view;
	yaw.SetToRotation(xoffset, 0.f, 1.f, 0.f);
	pitch.SetToRotation(yoffset, right.x, right.y, right.z);
	tempview = pitch * tempview;
	if (Math::RadianToDegree(acosf(tempview.y / tempview.Length())) >= 10.f && Math::RadianToDegree(acosf(tempview.y / tempview.Length())) <= 170.f)
	{
		view = tempview;
	}
	view = yaw * view;
	right = yaw * right;
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();

}

void cameratpp::UpdateMovement(const Vector3& pos, const Vector3& target)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
//	static const float CAMERA_SPEED = 50.f;
}

void  cameratpp::UpdateTurningMovement(bool bLeft, bool bRight, double dt)
{
	static const float CAMERA_SPEED = 25.f;
	if (bLeft)
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 1, 0, 0);
		position = rotation * position;
		//up = rotation * up;
	}
	if (bRight)
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 0, 1);
		position = rotation * position;
		//up = rotation * up;
	}
}
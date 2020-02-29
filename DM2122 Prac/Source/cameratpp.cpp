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
	 right = view.Cross(up).Normalized();
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
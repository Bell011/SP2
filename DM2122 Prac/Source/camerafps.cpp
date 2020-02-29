#include "camerafps.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"

camerafps::camerafps()
{
}

camerafps::~camerafps()
{
}

void camerafps::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	view = target - position;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	newcursorx = 960;
	newcursory = 540;
	SetCursorPos(newcursorx, newcursory);
}

void camerafps::Update(double dt)
{
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}

	mouse_callback();
}

void camerafps::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
void camerafps::mouse_callback()
{
	right = view.Cross(up).Normalized();
	POINT mousePos;
	GetCursorPos(&mousePos);
	int xposition = mousePos.x;
	int yposition = mousePos.y;

	float xoffset = newcursorx - xposition;
	float yoffset = newcursory - yposition;

	float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Mtx44 yaw;
	Mtx44 pitch;
	Vector3 temp = view;
	yaw.SetToRotation(xoffset, 0.f, 1.f, 0.f);
	pitch.SetToRotation(yoffset, right.x, right.y, right.z);
	temp = pitch * temp;
	if (Math::RadianToDegree(acosf(temp.y / temp.Length())) >= 10.f && Math::RadianToDegree(acosf(temp.y / temp.Length())) <= 170.f)
	{
		view = temp;
	}
	view = yaw * view;
	right = yaw * right;
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	target = position + view;
	SetCursorPos(newcursorx, newcursory);
}
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
	view = (target - position).Normalized();
	right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	yaw = -90.0f;
	pitch = 0.0f;
	fov = 180.0f;

	// mouse state
	firstMouse = true;
	lastX = 800.0f / 2.0;
	lastY = 600.0f / 2.0;
}

void camerafps::Update(double dt)
{
	static const float CAMERA_SPEED = 10.f;
	if (Application::IsKeyPressed('A'))
	{
		//float yaw = (float)(-CAMERA_SPEED * dt);
		//Mtx44 rotation;
		//rotation.SetToRotation(yaw, 0, 1, 0);
		position = position - right * dt * CAMERA_SPEED;
		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		//float yaw = (float)(CAMERA_SPEED * dt);
		//Mtx44 rotation;
		//rotation.SetToRotation(yaw, 0, 1, 0);
		//position = rotation * position;
		//up = rotation * up;
		position = position + right * dt * CAMERA_SPEED;
		target = position + view;
	}
	if (Application::IsKeyPressed('W'))
	{
		//float pitch = (float)(-CAMERA_SPEED * dt);
		//Vector3 view = (target - position).Normalized();
		//Vector3 right = view.Cross(up);
		//right.y = 0;
		//right.Normalize();
		//up = right.Cross(view).Normalized();
		//Mtx44 rotation;
		//rotation.SetToRotation(pitch, right.x, right.y, right.z);
		//position = rotation * position;
		position = position + view * dt * CAMERA_SPEED;
		target = position + view;
	}
	if (Application::IsKeyPressed('S'))
	{
		//float pitch = (float)(CAMERA_SPEED * dt);
		//Vector3 view = (target - position).Normalized();
		//Vector3 right = view.Cross(up);
		//right.y = 0;
		//right.Normalize();
		//up = right.Cross(view).Normalized();
		//Mtx44 rotation;
		//rotation.SetToRotation(pitch, right.x, right.y, right.z);
		//position = rotation * position;
		position = position - view * dt * CAMERA_SPEED;
		target = position + view;

	}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void camerafps::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
void camerafps::mouse_callback()
{
	view = (target - position).Normalized();
	right = view.Cross(up).Normalized();
	POINT mousePos;
	GetCursorPos(&mousePos);
	int xpos = mousePos.x;
	int ypos = mousePos.y;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	Vector3 front;
	front.x = cos(Math::DegreeToRadian(yaw)) * cos(Math::DegreeToRadian(pitch));
	front.y = sin(Math::DegreeToRadian(pitch));
	front.z = sin(Math::DegreeToRadian(yaw)) * cos(Math::DegreeToRadian(pitch));

	view = front.Normalized();
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();

	target = position + view;
}
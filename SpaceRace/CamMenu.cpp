#include "CamMenu.h"
#include "Application.h"
#include "Mtx44.h"

CamMenu::CamMenu()
{
}

CamMenu::~CamMenu()
{
}

void CamMenu::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	position = -view * 25.0f;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void CamMenu::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;

	view = target - position;
	right = view.Cross(up);
}

void CamMenu::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
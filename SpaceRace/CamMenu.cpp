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
	this->up = defaultUp = up;
}

void CamMenu::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
}
void CamMenu::SetPosition(float _x, float _y, float _z)
{
	position.x = _x;
	position.y = _y;
	position.z = _z;
}


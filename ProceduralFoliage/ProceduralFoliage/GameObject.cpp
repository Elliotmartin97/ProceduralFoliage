#include "GameObject.h"

GameObject::GameObject()
{
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;
	rotation.w = 1.0f;
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;
	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;
}

GameObject::~GameObject()
{
}

void GameObject::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

XMFLOAT3 GameObject::GetPosition()
{
	return position;
}

void GameObject::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

XMFLOAT4 GameObject::GetRotation()
{
	return rotation;
}

void GameObject::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

XMFLOAT3 GameObject::GetScale()
{
	return scale;
}
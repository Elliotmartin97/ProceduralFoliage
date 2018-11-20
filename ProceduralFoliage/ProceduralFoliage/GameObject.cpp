#include "GameObject.h"

GameObject::GameObject()
{
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f, 1.0f);
	SetScale(1.0f, 1.0f, 1.0f);

	transform = { 1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f };
	
}

GameObject::~GameObject()
{
}

void GameObject::MoveUP(float unit)
{
	XMVECTOR forward = transform.r[1];
	position = position + forward * unit;

	UpdateTransform();
}

void GameObject::SetPosition(float x, float y, float z)
{
	XMFLOAT3 pos_f = { x, y, z };
	position = XMLoadFloat3(&pos_f);
	UpdateTransform();
}

XMVECTOR GameObject::GetPosition()
{
	return position;
}

void GameObject::SetRotation(float x, float y, float z, float w)
{
	XMFLOAT4 rot_f = { x, y, z, w };
	rotation = XMLoadFloat4(&rot_f);
	UpdateTransform();
}

XMVECTOR GameObject::GetRotation()
{
	return rotation;
}

void GameObject::SetScale(float x, float y, float z)
{
	XMFLOAT3 sca_f = { x, y, z };
	scale = XMLoadFloat3(&sca_f);
	UpdateTransform();
}

XMVECTOR GameObject::GetScale()
{
	return scale;
}

void GameObject::UpdateTransform()
{
	//XMVECTOR rot_norm = XMVector4Normalize(rotation);

	XMMATRIX rot = XMMatrixRotationRollPitchYawFromVector(rotation);
	XMMATRIX pos = XMMatrixTranslationFromVector(position);
	XMMATRIX sca = XMMatrixScalingFromVector(scale);

	transform = sca * rot * pos;
}

XMMATRIX GameObject::GetTransform()
{
	UpdateTransform();
	return transform;
}

void GameObject::SetTransform(XMMATRIX new_transform)
{
	transform = new_transform;
	UpdateTransform();
}
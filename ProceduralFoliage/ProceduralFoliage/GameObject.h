#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class GameObject
{
public:

	GameObject();
	~GameObject();

	void MoveUP(float unit);

	void SetPosition(float, float, float);
	XMVECTOR GetPosition();

	void SetRotation(float, float, float, float);
	XMVECTOR GetRotation();

	void SetScale(float, float, float);
	XMVECTOR GetScale();

	void UpdateTransform();
	XMMATRIX GetTransform();
	void SetTransform(XMMATRIX new_transform);
private:
	XMMATRIX transform;
	XMVECTOR position;
	XMVECTOR rotation;
	XMVECTOR scale;
};
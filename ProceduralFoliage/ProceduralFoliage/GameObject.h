#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class GameObject
{
public:

	GameObject();
	~GameObject();

	void SetPosition(float, float, float);
	XMFLOAT3 GetPosition();

	void SetRotation(float, float, float);
	XMFLOAT4 GetRotation();

	void SetScale(float, float, float);
	XMFLOAT3 GetScale();

private:
	XMMATRIX transform;
	XMFLOAT3 position;
	XMFLOAT4 rotation;
	XMFLOAT3 scale;
};
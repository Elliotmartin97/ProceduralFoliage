#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class GameObject
{
public:

	GameObject();
	~GameObject();

	void MoveUp(float unit);
	void MoveDown(float unit);
	void MoveRight(float unit);
	void MoveLeft(float unit);
	void MoveForward(float unit);
	void MoveBackward(float unit);

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
	XMMATRIX identity_matrix;
	XMMATRIX transform;
	XMVECTOR position;
	XMVECTOR rotation;
	XMVECTOR scale;
};
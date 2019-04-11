#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <Windows.h>
#include <directxmath.h>
#include "Input.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void UpdateCameraInput(Input*);
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMVECTOR GetTransformedVectorPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	float pos_x, pos_y, pos_z;
	float rot_x, rot_y, rot_z;
	XMMATRIX view_matrix;

};

#endif
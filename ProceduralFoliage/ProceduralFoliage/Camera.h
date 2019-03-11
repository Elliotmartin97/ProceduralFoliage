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
	float m_position_x, m_position_y, m_position_z;
	float m_rotation_x, m_rotation_y, m_rotation_z;
	XMMATRIX m_view_matrix;

};

#endif
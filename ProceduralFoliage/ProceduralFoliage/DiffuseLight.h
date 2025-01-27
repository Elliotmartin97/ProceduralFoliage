#pragma once
#ifndef _DIFFUSELIGHT_H_
#define _DIFFUSELIGHT_H_

#include <DirectXMath.h>
using namespace DirectX;

class DiffuseLight
{
public:
	DiffuseLight();
	~DiffuseLight();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

private:
	XMFLOAT4 ambient_col;
	XMFLOAT4 diffuse_col;
	XMFLOAT3 m_direction;
};

#endif
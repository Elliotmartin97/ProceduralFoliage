#include "DiffuseLight.h"

DiffuseLight::DiffuseLight()
{
}

DiffuseLight::~DiffuseLight()
{
}

void DiffuseLight::SetAmbientColor(float red, float green, float blue, float alpha)
{
	ambient_col = XMFLOAT4(red, green, blue, alpha);
}

void DiffuseLight::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	diffuse_col = XMFLOAT4(red, green, blue, alpha);
}

void DiffuseLight::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
}

XMFLOAT4 DiffuseLight::GetAmbientColor()
{
	return ambient_col;
}

XMFLOAT4 DiffuseLight::GetDiffuseColor()
{
	return diffuse_col;
}

XMFLOAT3 DiffuseLight::GetDirection()
{
	return m_direction;
}
#include "DiffuseLight.h"

DiffuseLight::DiffuseLight()
{
}

DiffuseLight::~DiffuseLight()
{
}

void DiffuseLight::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void DiffuseLight::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void DiffuseLight::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

XMFLOAT4 DiffuseLight::GetAmbientColor()
{
	return m_ambientColor;
}

XMFLOAT4 DiffuseLight::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 DiffuseLight::GetDirection()
{
	return m_direction;
}
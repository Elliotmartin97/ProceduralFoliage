#include "Camera.h"

Camera::Camera()
{
	pos_x = 0.0f;
	pos_y = 0.0f;
	pos_z = 0.0f;

	rot_x = 0.0f;
	rot_y = 0.0f;
	rot_z = 0.0f;
}


Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
}


void Camera::SetRotation(float x, float y, float z)
{
	rot_x = x;
	rot_y = y;
	rot_z = z;
}

XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(pos_x, pos_y, pos_z);
}

XMVECTOR Camera::GetTransformedVectorPosition()
{
	XMFLOAT3 float_pos = XMFLOAT3(pos_x, pos_y, pos_z);
	XMVECTOR pos = XMLoadFloat3(&float_pos);

	return pos;
}

XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(rot_x, rot_y, rot_z);
}

void Camera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR up_vector, position_vector, look_at_vector;
	float yaw, pitch, roll;
	XMMATRIX rotation_matrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	up_vector = XMLoadFloat3(&up);

	position.x = pos_x;
	position.y = pos_y;
	position.z = pos_z;
	position_vector = XMLoadFloat3(&position);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	look_at_vector = XMLoadFloat3(&lookAt);

	pitch = rot_x * 0.0174532925f;
	yaw = rot_y * 0.0174532925f;
	roll = rot_z * 0.0174532925f;

	rotation_matrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	look_at_vector = XMVector3TransformCoord(look_at_vector, rotation_matrix);
	up_vector = XMVector3TransformCoord(up_vector, rotation_matrix);
	look_at_vector = XMVectorAdd(position_vector, look_at_vector);

	view_matrix = XMMatrixLookAtLH(position_vector, look_at_vector, up_vector);
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = view_matrix;
}

void Camera::UpdateCameraInput(Input* input)
{
	//position
	if (input->IsKeyDown(VK_UP))
	{
		pos_y += 0.1f;
	}
	if (input->IsKeyDown(VK_DOWN))
	{
		pos_y -= 0.1f;
	}
	if (input->IsKeyDown(VK_RIGHT))
	{
		pos_x += 0.1f;
	}
	if (input->IsKeyDown(VK_LEFT))
	{
		pos_x -= 0.1f;
	}

	//rotation
	if (input->IsKeyDown(0x57))
	{
		rot_x += 0.5f;
	}
	if (input->IsKeyDown(0x53))
	{
		rot_x -= 0.5f;
	}
	if (input->IsKeyDown(0x44))
	{
		rot_y += 0.5f;
	}
	if (input->IsKeyDown(0x41))
	{
		rot_y -= 0.5f;
	}
}
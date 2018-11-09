#include "Camera.h"

Camera::Camera()
{
	m_position_x = 0.0f;
	m_position_y = 0.0f;
	m_position_z = 0.0f;

	m_rotation_x = 0.0f;
	m_rotation_y = 0.0f;
	m_rotation_z = 0.0f;
}


Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position_x = x;
	m_position_y = y;
	m_position_z = z;
}


void Camera::SetRotation(float x, float y, float z)
{
	m_rotation_x = x;
	m_rotation_y = y;
	m_rotation_z = z;
}

XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(m_position_x, m_position_y, m_position_z);
}

XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(m_rotation_x, m_rotation_y, m_rotation_z);
}

void Camera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = m_position_x;
	position.y = m_position_y;
	position.z = m_position_z;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation_x * 0.0174532925f;
	yaw = m_rotation_y * 0.0174532925f;
	roll = m_rotation_z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_view_matrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_view_matrix;
}

void Camera::UpdateCameraInput(Input* input)
{
	//position
	if (input->IsKeyDown(VK_UP))
	{
		m_position_y += 0.1f;
	}
	if (input->IsKeyDown(VK_DOWN))
	{
		m_position_y -= 0.1f;
	}
	if (input->IsKeyDown(VK_RIGHT))
	{
		m_position_x += 0.1f;
	}
	if (input->IsKeyDown(VK_LEFT))
	{
		m_position_x -= 0.1f;
	}

	//rotation
	if (input->IsKeyDown(0x57))
	{
		m_rotation_x += 0.5f;
	}
	if (input->IsKeyDown(0x53))
	{
		m_rotation_x -= 0.5f;
	}
	if (input->IsKeyDown(0x44))
	{
		m_rotation_y += 0.5f;
	}
	if (input->IsKeyDown(0x41))
	{
		m_rotation_y -= 0.5f;
	}
}
#include "Camera.hpp"

Camera::Camera() {

}

Camera::~Camera() {

}

void Camera::setPosition(Vec3<float> pos) {
	m_Pos = pos;
}

void Camera::setRotation(Vec3<float> rot) {
	m_Rot = rot;
}

D3DXVECTOR3 Camera::getPosition() {
	return D3DXVECTOR3(m_Pos.x, m_Pos.y, m_Pos.z);
}

D3DXVECTOR3 Camera::getRotation() {
	return D3DXVECTOR3(m_Rot.x, m_Rot.y, m_Rot.z);
}

void Camera::render() {
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = m_Pos.x;
	position.y = m_Pos.y;
	position.z = m_Pos.z;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	pitch = m_Rot.x * 0.0174532925f;
	yaw = m_Rot.y * 0.0174532925f;
	roll = m_Rot.z * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
}
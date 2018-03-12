#include "Camera.hpp"

Camera::Camera() {
	D3DXMatrixIdentity(&m_Orientation);
}

Camera::~Camera() {

}

void Camera::setPosition(D3DXVECTOR3 pos) {
	m_Pos = pos;
}

void Camera::move(float v) {
	m_Pos += getForward() * v;
}

void Camera::rotate(D3DXVECTOR3 r) {
	D3DXMATRIX rotX, rotY, rotZ;

	D3DXMatrixRotationX(&rotX, r.x);
	D3DXMatrixRotationY(&rotY, r.y);
	D3DXMatrixRotationZ(&rotZ, r.z);

	m_Orientation *= rotY * rotX * rotZ;
}

D3DXVECTOR3 Camera::getPosition() {
	return m_Pos;
}

void Camera::render() {
	D3DXVECTOR3 up, position, forward;

	up		 = getUp();
	forward  = getForward();
	position = m_Pos;

	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &forward, &up);
}
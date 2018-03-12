#pragma once

#include <d3dx10math.h>

#include "Vec3.hpp"

class Camera {
	public:
		Camera();
		~Camera();

		void setPosition(D3DXVECTOR3 pos);

		void move(float v);
		void rotate(D3DXVECTOR3 r);

		D3DXVECTOR3 getRight()   { return D3DXVECTOR3(m_Orientation(0, 0), m_Orientation(0, 1), m_Orientation(0, 2)); }
		D3DXVECTOR3 getUp()      { return D3DXVECTOR3(m_Orientation(1, 0), m_Orientation(1, 1), m_Orientation(1, 2)); }
		D3DXVECTOR3 getForward() { return D3DXVECTOR3(m_Orientation(2, 0), m_Orientation(2, 1), m_Orientation(2, 2)); }

		Vec3<float> getPosition();

		void render();
		void getViewMatrix(D3DXMATRIX &m) { m = m_viewMatrix; }

	private:
		D3DXVECTOR3 m_Pos;
		D3DXMATRIX m_Orientation;
		D3DXMATRIX m_viewMatrix;
};

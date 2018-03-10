#pragma once

#include <d3dx10math.h>

#include "Vec3.hpp"

class Camera {
	public:
		Camera();
		~Camera();

		void setPosition(Vec3<float> pos);
		void setRotation(Vec3<float> pos);

		void move(Vec3<float> v) { m_Pos += v; }

		D3DXVECTOR3 getPosition();
		D3DXVECTOR3 getRotation();

		void render();
		void getViewMatrix(D3DXMATRIX &m) { m = m_viewMatrix; }

	private:
		Vec3<float> m_Pos, m_Rot;

		D3DXMATRIX m_viewMatrix;
};

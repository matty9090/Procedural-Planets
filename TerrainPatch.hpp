#pragma once

#include "Primitive.hpp"

class TerrainPatch : public Primitive {
	public:
		TerrainPatch(int face, Rect bounds);
		~TerrainPatch();

		bool init(ID3D11Device *device, Shader *shader);
		void cleanup();

		float getDiameter() { return m_Diameter; }
		Vec3<float> getCenterPos();

		enum EFaces { Top, Bottom, Right, Left, Front, Back };

	private:
		int m_FaceID;
		int m_GridSize;
		float m_Diameter;

		Rect m_Bounds;

		D3DXMATRIX m_MovMatrix;
		D3DXMATRIX m_PosMatrix;
		D3DXMATRIX m_RX, m_RY, m_RZ;

		void rot(Vec3<float> r);
		void mov(Vec3<float> m);

		D3DXVECTOR3 mapPointToSphere(D3DXVECTOR3 p);
		D3DXVECTOR3 vectorTransform(D3DXVECTOR3 v, D3DXMATRIX m);
};

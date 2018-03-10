#pragma once

#include <DirectXMath.h>

#include "Primitive.hpp"
#include "Shader.hpp"

class TerrainFace : public Primitive {
	public:
		TerrainFace(int faceID);
		~TerrainFace();

		bool init(ID3D11Device *device, Shader *shader);
		void cleanup();

		enum EFaces { Top, Bottom, Right, Left, Front, Back };

	private:
		int m_FaceID;

		D3DXMATRIX m_MovMatrix;
		D3DXMATRIX m_PosMatrix;
		D3DXVECTOR3 mapPointToSphere(D3DXVECTOR3 p);

		D3DXMATRIX m_RX, m_RY, m_RZ;

		void rot(Vec3<float> r);
		void mov(Vec3<float> m);

		D3DXVECTOR3 vectorTransform(D3DXVECTOR3 v, D3DXMATRIX m);
};

#pragma once

#include "Primitive.hpp"
#include "Terrain.hpp"

class TerrainPatch : public Primitive {
	public:
		TerrainPatch(Terrain *terrain, int face, Rect bounds, float radius);
		~TerrainPatch();

		bool init(ID3D11Device *device, Shader *shader);
		void cleanup();

		float getDiameter() { return m_Diameter; }
		Vec3<float> getCenterPos() { return m_HalfPos; }
		Vec3<float> getNormal() { return m_Normal; }

		enum EFaces { Top, Bottom, Right, Left, Front, Back };

	private:
		int m_FaceID;
		int m_GridSize;

		float m_Radius;
		float m_Diameter;

		Rect m_Bounds;
		Terrain *m_Terrain;

		Vec3<float> m_HalfPos, m_Normal;
		D3DXMATRIX m_LMovMatrix;
		D3DXMATRIX m_LRX, m_LRY, m_LRZ;

		void rot(Vec3<float> r);
		void mov(Vec3<float> m);

		D3DXVECTOR3 mapPointToSphere(D3DXVECTOR3 p);
		D3DXVECTOR3 vectorTransform(D3DXVECTOR3 v, D3DXMATRIX m);
};

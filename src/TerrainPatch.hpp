#pragma once

#include "Primitive.hpp"
#include "Terrain.hpp"

class TerrainNode;

class TerrainPatch : public Primitive {
	public:
		TerrainPatch(Terrain *terrain, TerrainNode* node, int face, Rect bounds, float radius);
		~TerrainPatch();

		bool init(ID3D11Device *device, Shader *shader);
		void cleanup();

		float getScale() { return m_Scale; }
		float getDiameter() { return m_Diameter; }

		Vec3<float> getCenterPos() { return m_HalfPos; }
		Vec3<float> getNormal() { return m_Normal; }

		std::vector<Primitive*> &getMarkers() { return m_Markers; }

		enum EFaces { Top, Bottom, Right, Left, Front, Back };
		enum EEdges { EdgeTop, EdgeRight, EdgeBottom, EdgeLeft };

	private:
		int m_FaceID;
		int m_GridSize;

		float m_Scale;
		float m_Radius;
		float m_Diameter;

		Rect m_Bounds;
		Terrain *m_Terrain;
		TerrainNode *m_Node;

		std::vector<Primitive*> m_Markers;

		Vec3<float> m_HalfPos, m_Normal;
		std::vector<std::vector<int>> m_Edges;

		D3DXMATRIX m_LMovMatrix;
		D3DXMATRIX m_LRX, m_LRY, m_LRZ;		

		void averageEdges(std::vector<Vertex> &vertices, std::vector<int> &points, int start, int end, int diff, int &count);
		void fixDetailDifference(std::vector<Vertex> &vertices);
		void createPlane(std::vector<Vertex> &vertices, std::vector<unsigned long> &indices);
		void calculateNormals(std::vector<Vertex> &vertices, std::vector<unsigned long> &indices);

		void rot(Vec3<float> r);
		void mov(Vec3<float> m);

		D3DXVECTOR3 mapPointToSphere(D3DXVECTOR3 p);
		D3DXVECTOR3 vectorTransform(D3DXVECTOR3 v, D3DXMATRIX m);
};

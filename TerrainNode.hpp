#pragma once

#include "TerrainPatch.hpp"

class TerrainNode {
	public:
		TerrainNode(TerrainNode *parent, Rect bounds, int face);
		~TerrainNode();

		void init(ID3D11Device *device, Shader *shader);
		void render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);
		void update(D3DXVECTOR3 cameraPos);
		void cleanup();
		
		void split();
		void merge();

		bool isLeaf() { return m_IsLeaf; }

	private:
		Rect m_Bounds;

		TerrainNode *m_Parent;
		TerrainNode *m_NW, *m_NE, *m_SE, *m_SW;
		TerrainPatch *m_Patch;

		enum EQuadrants { NorthEast, NorthWest, SouthEast, SouthWest };

		int m_FaceID;
		int m_Depth;
		bool m_IsLeaf;
};

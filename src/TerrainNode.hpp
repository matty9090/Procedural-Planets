#pragma once

#include "TerrainPatch.hpp"
#include "Camera.hpp"
#include "Terrain.hpp"
#include "Primitive.hpp"

class TerrainNode {
	public:
		TerrainNode(Terrain *terrain, TerrainNode *parent, Rect bounds, int face, Camera *cam, float radius);
		~TerrainNode();

		void init(ID3D11Device *device, Shader *shader);
		void render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXVECTOR3 camPos, D3DXVECTOR3 lightPos, D3DXVECTOR3 lightCol, D3DXVECTOR3 ambientColour);
		void connect(TerrainNode *north, TerrainNode *east, TerrainNode *south, TerrainNode *west);
		void update();
		void cleanup();
		
		void split();
		void merge();

		int getDepth() { return m_Depth; }
		bool isLeaf() { return m_IsLeaf; }
		TerrainPatch *getPatch() { return m_Patch; }

	private:
		Rect m_Bounds;

		Camera *m_Camera;

		Terrain *m_Terrain;
		TerrainPatch *m_Patch;

		TerrainNode *m_Parent;
		TerrainNode *m_NW, *m_NE, *m_SE, *m_SW;
		TerrainNode *m_NorthNhbr, *m_EastNhbr, *m_SouthNhbr, *m_WestNhbr;

		enum EQuadrants { NorthEast, NorthWest, SouthEast, SouthWest };

		int m_FaceID;
		int m_Depth;
		bool m_IsLeaf, m_IsVisible;
		float m_Radius;
};

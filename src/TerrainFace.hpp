#pragma once

#include "Primitive.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "TerrainNode.hpp"

class Terrain;

class TerrainFace {
	public:
		TerrainFace(Terrain *terrain, int faceID, Camera *cam, float radius);
		~TerrainFace();

		bool init(ID3D11Device *device, Shader *shader);
		void render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXVECTOR3 camPos, D3DXVECTOR3 lightPos, D3DXVECTOR3 lightCol, D3DXVECTOR3 ambientColour);
		void connect(TerrainFace *north, TerrainFace *east, TerrainFace *south, TerrainFace *west);
		void update();
		void split();
		void merge();
		void cleanup();

		TerrainNode *getQuadTree() { return m_Node; }

		enum EFaces { Top, Bottom, Right, Left, Front, Back };

	private:
		int m_FaceID;
		bool m_Visible;

		Camera *m_Camera;
		TerrainNode *m_Node;
		TerrainFace *m_NorthNhbr, *m_EastNhbr, *m_SouthNhbr, *m_WestNhbr;
};

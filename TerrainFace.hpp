#pragma once

#include "Primitive.hpp"
#include "Shader.hpp"
#include "TerrainNode.hpp"

class TerrainFace {
	public:
		TerrainFace(int faceID);
		~TerrainFace();

		bool init(ID3D11Device *device, Shader *shader);
		void render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);
		void cleanup();

		enum EFaces { Top, Bottom, Right, Left, Front, Back };

	private:
		int m_FaceID;

		TerrainNode *m_Node;
};

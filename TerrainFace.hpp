#pragma once

#include "Primitive.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "TerrainNode.hpp"

class TerrainFace {
	public:
		TerrainFace(int faceID, Camera *cam, float radius);
		~TerrainFace();

		bool init(ID3D11Device *device, Shader *shader);
		void render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);
		void update();
		void split();
		void merge();
		void cleanup();

		enum EFaces { Top, Bottom, Right, Left, Front, Back };

	private:
		int m_FaceID;

		Camera *m_Camera;
		TerrainNode *m_Node;
};

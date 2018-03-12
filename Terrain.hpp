#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include <vector>

#include "Camera.hpp"
#include "Shader.hpp"
#include "TerrainFace.hpp"

class Terrain {
	public:
		Terrain(ID3D11Device *device, ID3D11DeviceContext *deviceContext, Shader *shader, Camera *cam);
		~Terrain();

		bool init();
		void render(D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);
		void update();
		void cleanup();

	private:
		ID3D11Device *m_Device;
		ID3D11DeviceContext *m_DeviceContext;

		Camera *m_Camera;
		Shader *m_Shader;

		std::vector<TerrainFace*> faces;
};

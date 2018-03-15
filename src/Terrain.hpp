#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include <vector>

#include "Camera.hpp"
#include "Shader.hpp"
#include "SimplexNoise.hpp"
#include "Graphics.hpp"

class TerrainFace;

class Terrain {
	public:
		Terrain(ID3D11Device *device, ID3D11DeviceContext *deviceContext, Graphics *graphics, Shader *shader, Camera *cam, float radius);
		~Terrain();

		bool init();
		void render(D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXVECTOR3 camPos, D3DXVECTOR3 lightPos, D3DXVECTOR3 lightCol, D3DXVECTOR3 ambientColour);
		void update();
		void cleanup();

		float getHeight(Vec2<float> pos);

		Camera *getCamera() { return m_Camera; }
		Graphics *getGraphics() { return m_Graphics; }

	private:
		ID3D11Device *m_Device;
		ID3D11DeviceContext *m_DeviceContext;

		float m_Radius;

		Camera *m_Camera;
		Shader *m_Shader;
		Graphics *m_Graphics;
		SimplexNoise *m_Noise;

		std::vector<TerrainFace*> faces;
};

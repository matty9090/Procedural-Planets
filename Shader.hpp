#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

class Shader {
	public:
		Shader();
		~Shader();

		bool init(ID3D11Device *device, HWND hwnd);
		void cleanup();
		bool render(ID3D11DeviceContext *deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);


	private:
		struct MatrixBuffer {
			D3DXMATRIX world;
			D3DXMATRIX view;
			D3DXMATRIX projection;
		};

		ID3D11VertexShader *m_vertexShader;
		ID3D11PixelShader *m_pixelShader;
		ID3D11InputLayout *m_layout;
		ID3D11Buffer *m_matrixBuffer;
		
		bool setParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
};

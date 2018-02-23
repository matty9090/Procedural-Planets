#pragma once

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

#include "Camera.hpp"
#include "Shader.hpp"
#include "Primitive.hpp"

class Window;

class Graphics {
	public:
		Graphics(Window *window);
		~Graphics();

		bool init();

		void beginScene();
		void endScene();

		void GetProjMatrix(D3DXMATRIX &m) { m = m_ProjectionMatrix; }
		void GetWorldMatrix(D3DXMATRIX &m) { m = m_WorldMatrix; }
		void GetOrthoMatrix(D3DXMATRIX &m) { m = m_OrthoMatrix; }

	private:
		Window *m_Window;

		size_t m_ViewportW, m_ViewportH;

		bool m_vsync_enabled;
		int m_videoCardMemory;
		char m_videoCardDescription[128];
		float m_Near, m_Far;

		IDXGISwapChain *m_SwapChain;
		ID3D11Device *m_Device;
		ID3D11DeviceContext *m_DeviceContext;
		ID3D11RenderTargetView *m_RenderTargetView;
		ID3D11Texture2D *m_DepthStencilBuffer;
		ID3D11DepthStencilState *m_DepthStencilState;
		ID3D11DepthStencilView *m_DepthStencilView;
		ID3D11RasterizerState *m_RasterState;

		D3DXMATRIX m_ProjectionMatrix, m_WorldMatrix, m_OrthoMatrix;

		Camera *m_Camera;
		Shader *m_Shader;
		Primitive *m_Primitive;

		bool initDevice();
		bool initDepthBuffer();
		bool initBackBuffer();
		bool initRaster();
		bool initViewport();
		bool initMatrices();
		
		void cleanup();
};


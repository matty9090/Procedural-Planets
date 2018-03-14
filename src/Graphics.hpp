#pragma once

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

#include <FW1FontWrapper.h>

class Window;

class Graphics {
	public:
		Graphics(Window *window);
		~Graphics();

		bool init();

		void beginScene();
		void endScene();

		void toggleWireframe();

		void GetProjMatrix(D3DXMATRIX &m) { m = m_ProjectionMatrix; }
		void GetOrthoMatrix(D3DXMATRIX &m) { m = m_OrthoMatrix; }

		void drawText(float x, float y, UINT32 colour, const WCHAR *str);

		D3DXMATRIX &getProjectionMatrix() { return m_ProjectionMatrix; }
		ID3D11Device *getDevice() { return m_Device; }
		ID3D11DeviceContext *getDeviceContext() { return m_DeviceContext; }

	private:
		Window *m_Window;

		size_t m_ViewportW, m_ViewportH;

		bool m_vsync_enabled;
		bool m_Wireframe;
		int m_videoCardMemory;
		char m_videoCardDescription[128];
		float m_Near, m_Far;

		IFW1FontWrapper *m_FontWrapper;
		IDXGISwapChain *m_SwapChain;
		ID3D11Device *m_Device;
		ID3D11DeviceContext *m_DeviceContext;
		ID3D11RenderTargetView *m_RenderTargetView;
		ID3D11Texture2D *m_DepthStencilBuffer;
		ID3D11DepthStencilState *m_DepthStencilState;
		ID3D11DepthStencilView *m_DepthStencilView;
		ID3D11RasterizerState *m_RasterState;

		D3DXMATRIX m_ProjectionMatrix, m_OrthoMatrix;

		bool initAdapter();
		bool initDevice();
		bool initDepthBuffer();
		bool initBackBuffer();
		bool initRaster(bool wireframe = false);
		bool initViewport();
		bool initMatrices();
		bool initFonts();
		
		void cleanup();
};


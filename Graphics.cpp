#include "Graphics.hpp"
#include "Window.hpp"

Graphics::Graphics(Window *window) : m_Window(window), m_Near(0.1f), m_Far(100.0f) {
	m_SwapChain = NULL;
	m_Device = NULL;
	m_DeviceContext = NULL;
	m_RenderTargetView = NULL;
	m_DepthStencilBuffer = NULL;
	m_DepthStencilState = NULL;
	m_DepthStencilView = NULL;
	m_RasterState = NULL;
}

Graphics::~Graphics() {

}

bool Graphics::init() { 
	if (!initDevice())      { MessageBoxA(m_Window->getHandle(), "Failed to init device",      "Error", MB_OK); return false; }
	if (!initBackBuffer())  { MessageBoxA(m_Window->getHandle(), "Failed to init back buffer", "Error", MB_OK); return false; }
	if (!initDepthBuffer()) { MessageBoxA(m_Window->getHandle(), "Failed to init buffer",      "Error", MB_OK); return false; }
	if (!initRaster())      { MessageBoxA(m_Window->getHandle(), "Failed to init raster",      "Error", MB_OK); return false; }
	if (!initMatrices())    { MessageBoxA(m_Window->getHandle(), "Failed to init matrices",    "Error", MB_OK); return false; }

	m_Camera = new Camera();
	m_Shader = new Shader();
	m_Primitive = new Primitive();

	m_Camera->setPosition(Vec3<float>(0, 0, 0));

	if (!m_Shader->init(m_Device, m_Window->getHandle())) return false;
	if (!m_Primitive->init(m_Device)) return false;

	return true;
}

void Graphics::beginScene() {
	D3DXMATRIX viewMatrix;
	
	float colour[4] = { 135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, colour);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_Camera->render();
	m_Camera->getViewMatrix(viewMatrix);
	m_Primitive->render(m_DeviceContext);
	m_Shader->render(m_DeviceContext, m_Primitive->getIndexCount(), m_WorldMatrix, viewMatrix, m_ProjectionMatrix);
}

void Graphics::endScene() {
	m_SwapChain->Present(0, 0);
}

bool Graphics::initDevice() {
	RECT rc;
	GetClientRect(m_Window->getHandle(), &rc);
	m_ViewportW = rc.right - rc.left;
	m_ViewportH = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC sd; 
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferCount = 1;
	sd.BufferDesc.Width = m_ViewportW;
	sd.BufferDesc.Height = m_ViewportH;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.OutputWindow = m_Window->getHandle();
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_Device, NULL, &m_DeviceContext)))
		return false;

	return true;
}

bool Graphics::initBackBuffer() {
	ID3D11Texture2D *pBackBuffer;

	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return false;

	if (FAILED(m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView)))
		return false;

	pBackBuffer->Release();

	return true;
}

bool Graphics::initDepthBuffer() {
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthBufferDesc.Width = m_Window->getWindowWidth();
	depthBufferDesc.Height = m_Window->getWindowHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if (FAILED(m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer)))
		return false;

	if (FAILED(m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState)))
		return false;

	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);

	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView)))
		return false;

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	return true;
}

bool Graphics::initRaster() {
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(m_Device->CreateRasterizerState(&rasterDesc, &m_RasterState)))
		return false;

	m_DeviceContext->RSSetState(m_RasterState);

	return true;
}

bool Graphics::initViewport() {
	D3D11_VIEWPORT viewport;

	viewport.Width = (float)m_Window->getWindowWidth();
	viewport.Height = (float)m_Window->getWindowHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_DeviceContext->RSSetViewports(1, &viewport);

	return false;
}

bool Graphics::initMatrices() {
	float fov = (float)D3DX_PI / 4.0f;
	float aspect = (float)m_Window->getWindowWidth() / (float)m_Window->getWindowHeight();

	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, fov, aspect, m_Near, m_Far);
	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixOrthoLH(&m_OrthoMatrix, (float)m_Window->getWindowWidth(), (float)m_Window->getWindowHeight(), m_Near, m_Far);

	return true;
}

void Graphics::cleanup() {
	if (m_SwapChain) m_SwapChain->SetFullscreenState(false, NULL);
	if (m_RasterState) m_RasterState->Release();
	if (m_DepthStencilView) m_DepthStencilView->Release();
	if (m_DepthStencilState) m_DepthStencilState->Release();
	if (m_DepthStencilBuffer) m_DepthStencilBuffer->Release();
	if (m_RenderTargetView) m_RenderTargetView->Release();
	if (m_DeviceContext) m_DeviceContext->Release();
	if (m_Device) m_Device->Release();
	if (m_SwapChain) m_SwapChain->Release();

	m_Primitive->cleanup();
	m_Shader->cleanup();

	delete m_Camera;
	delete m_Shader;
	delete m_Primitive;
}

#include "Graphics.hpp"
#include "Window.hpp"

Graphics::Graphics(Window *window) : m_Window(window), m_Near(0.1f), m_Far(1000.0f), m_Wireframe(false) {
	m_SwapChain = NULL;
	m_Device = NULL;
	m_DeviceContext = NULL;
	m_RenderTargetView = NULL;
	m_DepthStencilBuffer = NULL;
	m_DepthStencilState = NULL;
	m_DepthStencilView = NULL;
	m_RasterState = NULL;

	m_Shader = NULL;
	m_Camera = NULL;
	m_Terrain = NULL;
}

Graphics::~Graphics() {

}

bool Graphics::init() { 
	if (!initAdapter())     { MessageBoxA(m_Window->getHandle(), "Failed to init adapter",     "Error", MB_OK); return false; }
	if (!initDevice())      { MessageBoxA(m_Window->getHandle(), "Failed to init device",      "Error", MB_OK); return false; }
	if (!initBackBuffer())  { MessageBoxA(m_Window->getHandle(), "Failed to init back buffer", "Error", MB_OK); return false; }
	if (!initDepthBuffer()) { MessageBoxA(m_Window->getHandle(), "Failed to init buffer",      "Error", MB_OK); return false; }
	if (!initRaster())      { MessageBoxA(m_Window->getHandle(), "Failed to init raster",      "Error", MB_OK); return false; }
	if (!initViewport())    { MessageBoxA(m_Window->getHandle(), "Failed to init viewport",    "Error", MB_OK); return false; }
	if (!initMatrices())    { MessageBoxA(m_Window->getHandle(), "Failed to init matrices",    "Error", MB_OK); return false; }

	m_Camera  = new Camera();
	m_Shader  = new Shader();
	m_Terrain = new Terrain(m_Device, m_DeviceContext, m_Shader);

	m_Camera->setPosition(Vec3<float>(0, 0, -20.0f));

	if (!m_Shader->init(m_Device, m_Window->getHandle())) return false;
	if (!m_Terrain->init()) return false;

	return true;
}

void Graphics::render() {
	beginScene();

	D3DXMATRIX viewMatrix;

	m_Camera->render();
	m_Camera->getViewMatrix(viewMatrix);

	m_Terrain->render(viewMatrix, m_ProjectionMatrix);

	endScene();
}

void Graphics::beginScene() {
	float colour[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, colour);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::endScene() {
	m_SwapChain->Present(0, 0);
}

void Graphics::toggleWireframe() {
	m_Wireframe = !m_Wireframe;
	initRaster(m_Wireframe);
}

bool Graphics::initAdapter() {
	IDXGIFactory *factory;
	IDXGIAdapter *adapter;
	IDXGIOutput *adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC *displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory))) return false;
	if (FAILED(factory->EnumAdapters(0, &adapter))) return false;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput))) return false;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL))) return false;

	displayModeList = new DXGI_MODE_DESC[numModes];

	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList))) return false;

	for (i = 0; i<numModes; i++) {
		if (displayModeList[i].Width == (unsigned int)m_Window->getWindowWidth()) {
			if (displayModeList[i].Height == (unsigned int)m_Window->getWindowHeight()) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	if (FAILED(adapter->GetDesc(&adapterDesc))) return false;

	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	if (wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128) != 0) return false;

	delete[] displayModeList;

	adapterOutput->Release();
	adapter->Release();
	factory->Release();

	return true;
}

bool Graphics::initDevice() {
	RECT rc;
	GetClientRect(m_Window->getHandle(), &rc);
	m_ViewportW = rc.right - rc.left;
	m_ViewportH = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_Window->getWindowWidth();
	swapChainDesc.BufferDesc.Height = m_Window->getWindowHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.OutputWindow = m_Window->getHandle();
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext)))
		return false;

	return true;
}

bool Graphics::initBackBuffer() {
	ID3D11Texture2D *backBufferPtr;

	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr)))
		return false;

	if (FAILED(m_Device->CreateRenderTargetView(backBufferPtr, NULL, &m_RenderTargetView)))
		return false;

	backBufferPtr->Release();

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

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

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

bool Graphics::initRaster(bool wireframe) {
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = (wireframe) ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
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

	return true;
}

bool Graphics::initMatrices() {
	float fov = (float)D3DX_PI / 4.0f;
	float aspect = (float)m_Window->getWindowWidth() / (float)m_Window->getWindowHeight();

	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, fov, aspect, m_Near, m_Far);
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

	m_Terrain->cleanup();
	m_Shader->cleanup();

	delete m_Camera;
	delete m_Shader;
	delete m_Terrain;
}

#include "Shader.hpp"

#include <fstream>

Shader::Shader() {
	m_VertexShader = NULL;
	m_PixelShader = NULL;
	m_Layout = NULL;
	m_MatrixBuffer = NULL;
}

bool Shader::init(ID3D11Device *device, HWND hwnd) {
	ID3D10Blob *errorMessage = NULL;
	ID3DBlob *vertexShaderBuffer = NULL;
	ID3DBlob *pixelShaderBuffer = NULL;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	D3D11_BUFFER_DESC matrixBufferDesc;

	unsigned int numElements;

	D3DReadFileToBlob(L"Shaders/VertexShader.cso", &vertexShaderBuffer);
	D3DReadFileToBlob(L"Shaders/PixelShader.cso", &pixelShaderBuffer);

	if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader)))
		return false;

	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader)))
		return false;
	
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	if (FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_Layout)))
		return false;

	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer)))
		return false;

	return true;
}

void Shader::cleanup() {
	if (m_MatrixBuffer) { m_MatrixBuffer->Release(); delete m_MatrixBuffer; }
	if (m_Layout) { m_Layout->Release(); delete m_Layout; }
	if (m_PixelShader) { m_PixelShader->Release(); delete m_PixelShader; }
	if (m_VertexShader) { m_VertexShader->Release(); delete m_VertexShader; }
}

bool Shader::render(ID3D11DeviceContext *deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	if (!setParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix))
		return false;

	deviceContext->IASetInputLayout(m_Layout);
	deviceContext->VSSetShader(m_VertexShader, NULL, 0);
	deviceContext->PSSetShader(m_PixelShader, NULL, 0);
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return true;
}

bool Shader::setParameters(ID3D11DeviceContext *deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer* dataPtr;
	unsigned int bufferNumber;

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	if (FAILED(deviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	dataPtr = (MatrixBuffer*)mappedResource.pData;
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(m_MatrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_MatrixBuffer);

	return true;
}

void Shader::handleErrors(ID3D10Blob *errorMessage, HWND hwnd, char *shaderFilename) {
	char *compileErrors;
	unsigned long bufferSize, i;
	std::ofstream file("errors.txt");

	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();

	for (i = 0; i < bufferSize; i++)
		file << compileErrors[i];

	file.close();

	errorMessage->Release();
	errorMessage = 0;

	MessageBoxA(hwnd, "Error compiling shader, check errors.txt", shaderFilename, MB_OK);
}

Shader::~Shader() {

}

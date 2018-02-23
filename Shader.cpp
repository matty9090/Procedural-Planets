#include "Shader.hpp"

Shader::Shader() {
	m_vertexShader = NULL;
	m_pixelShader = NULL;
	m_layout = NULL;
	m_matrixBuffer = NULL;
}

bool Shader::init(ID3D11Device *device, HWND hwnd) {
	

	return true;
}

void Shader::cleanup() {

}

bool Shader::render(ID3D11DeviceContext * deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	return false;
}

bool Shader::setParameters(ID3D11DeviceContext *, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX) {
	return true;
}

Shader::~Shader() {

}

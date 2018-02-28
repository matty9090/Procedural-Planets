#include "Sphere.hpp"

Sphere::Sphere() {

}

Sphere::~Sphere() {
}

bool Sphere::init(ID3D11Device *device) {
	m_VertexCount = 8;
	m_IndexCount = 14;

	Vertex vertices[] = {
		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR4(1.0f, 0.3f, 0.3f, 0.0f) },
		{ D3DXVECTOR3(-1.0f,  1.0f, -1.0f), D3DXVECTOR4(1.0f, 0.5f, 0.5f, 0.0f) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR4(1.0f, 0.6f, 0.6f, 0.0f) },
		{ D3DXVECTOR3(1.0f,  1.0f, -1.0f), D3DXVECTOR4(1.0f, 0.8f, 0.8f, 0.0f) },
		{ D3DXVECTOR3(1.0f,  -1.0f, 1.0f), D3DXVECTOR4(1.0f, 0.5f, 0.5f, 0.0f) },
		{ D3DXVECTOR3(1.0f,  1.0f, 1.0f), D3DXVECTOR4(1.0f, 0.3f, 0.3f, 0.0f) },
		{ D3DXVECTOR3(-1.0f,  -1.0f, 1.0f), D3DXVECTOR4(1.0f, 0.5f, 0.5f, 0.0f) },
		{ D3DXVECTOR3(-1.0f,  1.0f, 1.0f), D3DXVECTOR4(1.0f, 0.3f, 0.3f, 0.0f) }
	};

	unsigned long indices[] = {
		1, 3, 0,
		2, 4, 3,
		5, 1, 7,
		0, 6, 4,
		7, 5
	};

	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixTranslation(&m_MatrixMov, 0, 0, 0);

	return initData(device, vertices, indices);
}

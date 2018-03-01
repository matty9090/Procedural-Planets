#include "Sphere.hpp"

Sphere::Sphere() {

}

Sphere::~Sphere() {
}

bool Sphere::init(ID3D11Device *device) {
	int steps = 10, slices = 10;

	m_VertexCount = steps * slices;
	m_IndexCount = (((m_VertexCount - 2) * 2)) + 2;

	Vertex *vertices = new Vertex[m_IndexCount];

	unsigned long *indices = new unsigned long(m_IndexCount);

	vertices[0] = { D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0) };

	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixTranslation(&m_MatrixMov, 0, 0, 0);

	if (initData(device, vertices, indices)) {
		cleanVertices(vertices, indices);
		return true;
	}

	return false;
}

void Sphere::cleanVertices(Vertex *vertices, unsigned long *indices) {
	delete[] vertices;
	delete[] indices;
}

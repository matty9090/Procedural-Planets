#include "Sphere.hpp"

Sphere::Sphere() {

}

Sphere::~Sphere() {
}

bool Sphere::init(ID3D11Device *device) {
	size_t size = 10;

	int v = 0, c = 0;

	auto face1 = generateGrid(Front, size);

	Vertex *vertices = new Vertex[m_VertexCount];
	unsigned long *indices = new unsigned long[m_IndexCount];

	for (auto i : face1.first)  vertices[v++] = i;
	for (auto i : face1.second) indices[c++] = (unsigned long)i;

	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixTranslation(&m_MatrixMov, -2.0f, -2.0f, 0.0f);
	m_WorldMatrix = m_MatrixMov;

	if (initData(device, vertices, indices)) {
		cleanVertices(vertices, indices);
		return true;
	}

	return false;
}

void Sphere::cleanVertices(Vertex *vertices, unsigned long *indices) {
	//delete[] vertices;
	//delete[] indices;
}

std::pair<std::vector<Primitive::Vertex>, std::vector<int>> Sphere::generateGrid(int face, size_t size) {
	std::vector<Vertex> v;
	std::vector<int> i;

	float step = 5.0f / size;

	switch (face) {
		case Top: case Bottom:
			for (float y = 0; y < 1; y += step)
				for (float x = 0; x < 1; x += step)
					v.push_back({ D3DXVECTOR3(x, (face == Top) ? 1.f : -1.f, y), D3DXVECTOR3(0, 0, 0), D3DXVECTOR4((rand() % 100) / 100.0f, 0.2f, 0.9f, 1.0f) });
			
			break;

		case Right: case Left:
			for (float y = 0; y < 1; y += step)
				for (float x = 0; x < 1; x += step)
					v.push_back({ D3DXVECTOR3((face == Right) ? -1.f : 1.f, x, y), D3DXVECTOR3(0, 0, 0), D3DXVECTOR4(0.0f, (rand() % 100) / 100.0f, 0.9f, 1.0f) });
			
			break;

		case Front: case Back:
			for (float y = 0; y < 5; y += step)
				for (float x = 0; x < 5; x += step)
					v.push_back({ D3DXVECTOR3(x, y, (face == Front) ? -1.f : 1.f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR4(0.0f, 0.2f, (rand() % 100) / 100.0f, 1.0f) });
			
			break;
	}

	for (size_t y = 0; y < size - 1; ++y) {
		if(y > 0)
			i.push_back(y * size);
		
		for (size_t x = 0; x < size; ++x) {
			i.push_back(y * size + x);
			i.push_back(((y + 1) * size) + x);
		}

		if(y < size - 2)
			i.push_back(((y + 1) * size) + (size - 1));
	}

	m_VertexCount = v.size();
	m_IndexCount = i.size();

	return std::make_pair(v, i);
}

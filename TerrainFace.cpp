#include "TerrainFace.hpp"

using namespace DirectX;

TerrainFace::TerrainFace(int id) : m_FaceID(id) {

}

bool TerrainFace::init(ID3D11Device *device, Shader *shader) {
	m_Shader = shader;

	int gsize = 50;
	float size = 1.0f;
	float step = size / (gsize - 1);

	std::vector<Vertex> vertices;
	std::vector<unsigned long> indices;

	for (int i = 0; i < gsize; i++) {
		float y = i * step - size / 2;

		for (int j = 0; j < gsize; j++) {
			float x = j * step - size / 2;

			vertices.push_back({ D3DXVECTOR3(x, y, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR4(0.0f, ((rand() % 100) / 100.0f) + 0.1f, 1.0f, 1.0f) });
		}
	}

	for (int y = 0; y < gsize - 1; y++) {
		for (int x = 0; x < gsize - 1; x++) {
			indices.push_back(y * gsize + x);
			indices.push_back(y * gsize + x + 1);
			indices.push_back((y + 1) * gsize + x);

			indices.push_back((y + 1) * gsize + x);
			indices.push_back(y * gsize + x + 1);
			indices.push_back((y + 1) * gsize + x + 1);
		}
	}

	m_VertexCount = vertices.size();
	m_IndexCount = indices.size();

	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixTranslation(&m_MatrixMov, 0, 0, 0);

	float bscale = 2.0f, ascale = 5.0f;

	D3DXMATRIX scaleMatrix;

	D3DXMatrixScaling(&m_ScaleMatrix, ascale, ascale, ascale);
	D3DXMatrixScaling(&scaleMatrix, bscale, bscale, bscale);

	switch (m_FaceID) {
		case Top:    rot(Vec3<float>(D3DX_PI / 2, 0.0f, 0.0f));  mov(Vec3<float>( 0.0f,   0.5f,  0.0f));  break;
		case Bottom: rot(Vec3<float>(D3DX_PI / 2, 0.0f, 0.0f));  mov(Vec3<float>( 0.0f,  -0.5f,  0.0f));  break;
		case Left:   rot(Vec3<float>(0.0f, D3DX_PI / 2, 0.0f));  mov(Vec3<float>(-0.5f,   0.0f,  0.0f));  break;
		case Right:  rot(Vec3<float>(0.0f, D3DX_PI / 2, 0.0f));  mov(Vec3<float>( 0.5f,   0.0f,  0.0f));  break;
		case Front:  rot(Vec3<float>(0.0f, 0.0f, 0.0f));         mov(Vec3<float>( 0.0f,   0.0f, -0.5f));  break;
		case Back:   rot(Vec3<float>(0.0f, 0.0f, 0.0f));         mov(Vec3<float>( 0.0f,   0.0f,  0.5f));  break;
	}

	m_PosMatrix = m_RZ * m_RX * m_RY * m_MovMatrix * scaleMatrix;

	for (auto &v : vertices) {
		XMMATRIX tm = XMMatrixSet(m_PosMatrix._11, m_PosMatrix._12, m_PosMatrix._13, m_PosMatrix._14,
						  	      m_PosMatrix._21, m_PosMatrix._22, m_PosMatrix._23, m_PosMatrix._24,
							      m_PosMatrix._31, m_PosMatrix._32, m_PosMatrix._33, m_PosMatrix._34,
								  m_PosMatrix._41, m_PosMatrix._42, m_PosMatrix._43, m_PosMatrix._44);
		
		XMVECTOR tv = XMVectorSet(v.position.x, v.position.y, v.position.z, 1.0);
		XMVECTOR vv = XMVector3Transform(tv, tm);
		XMFLOAT4 vc;
		XMStoreFloat4(&vc, vv);

		v.position = mapPointToSphere(D3DXVECTOR3(vc.x, vc.y, vc.z));
	}

	return initData(device, vertices, indices);
}

void TerrainFace::cleanup() {
	Primitive::cleanup();
}

D3DXVECTOR3 TerrainFace::mapPointToSphere(D3DXVECTOR3 p) {
	float x2 = p.x * p.x, y2 = p.y * p.y, z2 = p.z * p.z;

	return D3DXVECTOR3(p.x * sqrtf(1.0f - y2 * 0.5f - z2 * 0.5f + (y2 * z2) * 0.33333f),
					   p.y * sqrtf(1.0f - z2 * 0.5f - x2 * 0.5f + (z2 * x2) * 0.33333f),
					   p.z * sqrtf(1.0f - x2 * 0.5f - y2 * 0.5f + (x2 * y2) * 0.33333f));
}

void TerrainFace::rot(Vec3<float> r) {
	D3DXMatrixRotationX(&m_RX, r.x);
	D3DXMatrixRotationY(&m_RY, r.y);
	D3DXMatrixRotationZ(&m_RZ, r.z);
}

void TerrainFace::mov(Vec3<float> m) {
	D3DXMatrixTranslation(&m_MovMatrix, m.x, m.y, m.z);
}

TerrainFace::~TerrainFace() {
	delete m_Shader;
}

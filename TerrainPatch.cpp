#include "TerrainPatch.hpp"

#include <DirectXMath.h>

using namespace DirectX;

TerrainPatch::TerrainPatch(int face) : m_FaceID(face) {

}

bool TerrainPatch::init(ID3D11Device *device, Shader *shader) {
	m_Shader = shader;

	int gsize = 20;
	float size = 1.0f;
	float step = size / (gsize - 1);

	std::vector<Vertex> vertices;
	std::vector<unsigned long> indices;

	for (int i = 0; i < gsize; i++) {
		float y = i * step - size / 2;

		for (int j = 0; j < gsize; j++) {
			float x = j * step - size / 2;

			vertices.push_back({ D3DXVECTOR3(x, y, 0), D3DXVECTOR3(0, 0, 1), D3DXVECTOR4(0.0f, ((rand() % 100) / 100.0f) + 0.1f, 1.0f, 1.0f) });
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
		case Top:    rot(Vec3<float>((float)D3DX_PI / 2.0f, 0.0f, 0.0f));   mov(Vec3<float>(0.0f, 0.5f, 0.0f));  break;
		case Bottom: rot(Vec3<float>((float)D3DX_PI / 2.0f, 0.0f, 0.0f));   mov(Vec3<float>(0.0f, -0.5f, 0.0f));  break;
		case Left:   rot(Vec3<float>(0.0f, (float)D3DX_PI / 2.0f, 0.0f));   mov(Vec3<float>(-0.5f, 0.0f, 0.0f));  break;
		case Right:  rot(Vec3<float>(0.0f, (float)D3DX_PI / 2.0f, 0.0f));   mov(Vec3<float>(0.5f, 0.0f, 0.0f));  break;
		case Front:  rot(Vec3<float>(0.0f, 0.0f, 0.0f));					mov(Vec3<float>(0.0f, 0.0f, -0.5f));  break;
		case Back:   rot(Vec3<float>(0.0f, 0.0f, 0.0f));					mov(Vec3<float>(0.0f, 0.0f, 0.5f));  break;
	}

	m_PosMatrix = m_RZ * m_RX * m_RY * m_MovMatrix * scaleMatrix;

	for (auto &v : vertices) {
		v.position = mapPointToSphere(vectorTransform(v.position, m_PosMatrix));
		v.normal = vectorTransform(v.normal, m_PosMatrix);
	}

	return initData(device, vertices, indices);
}

void TerrainPatch::cleanup() {
	delete m_Shader;
}

D3DXVECTOR3 TerrainPatch::mapPointToSphere(D3DXVECTOR3 p) {
	float x2 = p.x * p.x, y2 = p.y * p.y, z2 = p.z * p.z;

	return D3DXVECTOR3(p.x * sqrtf(1.0f - y2 * 0.5f - z2 * 0.5f + (y2 * z2) * 0.33333f),
		p.y * sqrtf(1.0f - z2 * 0.5f - x2 * 0.5f + (z2 * x2) * 0.33333f),
		p.z * sqrtf(1.0f - x2 * 0.5f - y2 * 0.5f + (x2 * y2) * 0.33333f));
}

void TerrainPatch::rot(Vec3<float> r) {
	D3DXMatrixRotationX(&m_RX, r.x);
	D3DXMatrixRotationY(&m_RY, r.y);
	D3DXMatrixRotationZ(&m_RZ, r.z);
}

void TerrainPatch::mov(Vec3<float> m) {
	D3DXMatrixTranslation(&m_MovMatrix, m.x, m.y, m.z);
}

D3DXVECTOR3 TerrainPatch::vectorTransform(D3DXVECTOR3 v, D3DXMATRIX m) {
	XMMATRIX tm = XMMatrixSet(m._11, m._12, m._13, m._14,
						  	  m._21, m._22, m._23, m._24,
							  m._31, m._32, m._33, m._34,
							  m._41, m._42, m._43, m._44);

	XMFLOAT4 vc;
	XMVECTOR tv = XMVectorSet(v.x, v.y, v.z, 1.0);
	XMVECTOR vv = XMVector3Transform(tv, tm);
	XMStoreFloat4(&vc, vv);

	return D3DXVECTOR3(vc.x, vc.y, vc.z);
}

TerrainPatch::~TerrainPatch() {

}

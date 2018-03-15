#include "TerrainPatch.hpp"

#include <DirectXMath.h>

using namespace DirectX;

TerrainPatch::TerrainPatch(Terrain *terrain, int face, Rect bounds, float radius)
	: m_Terrain(terrain), m_FaceID(face), m_GridSize(8), m_Bounds(bounds), m_Radius(radius) {

}

bool TerrainPatch::init(ID3D11Device *device, Shader *shader) {
	Primitive::init(device, shader);

	m_Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float size_x = m_Bounds.x2 - m_Bounds.x;
	float size_y = m_Bounds.y2 - m_Bounds.y;

	float step_x = size_x / (m_GridSize - 1);
	float step_y = size_y / (m_GridSize - 1);

	std::vector<Vertex> vertices;
	std::vector<unsigned long> indices;

	m_Scale = sqrtf(size_x * size_x + size_y * size_y);

	for (int i = 0; i < m_GridSize; i++) {
		float y = i * step_y + m_Bounds.y;

		for (int j = 0; j < m_GridSize; j++) {
			float x = j * step_x + m_Bounds.x;

			D3DXVECTOR4 colour = D3DXVECTOR4(0.0f, 0.8f, 0.0f, 1.0f);
			vertices.push_back({ D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0, 0, 1), colour });
		}
	}

	for (int y = 0; y < m_GridSize - 1; y++) {
		for (int x = 0; x < m_GridSize - 1; x++) {
			indices.push_back(y * m_GridSize + x);
			indices.push_back(y * m_GridSize + x + 1);
			indices.push_back((y + 1) * m_GridSize + x);

			indices.push_back((y + 1) * m_GridSize + x);
			indices.push_back(y * m_GridSize + x + 1);
			indices.push_back((y + 1) * m_GridSize + x + 1);
		}
	}

	m_VertexCount = vertices.size();
	m_IndexCount  = indices.size();

	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixTranslation(&m_MatrixMov, 0, 0, 0);

	float scale = 2.0f;

	D3DXMATRIX sphereScale, localMatrix, radiusMatrix;

	D3DXMatrixScaling(&m_ScaleMatrix, 1.0f, 1.0f, 1.0f);
	D3DXMatrixScaling(&sphereScale, scale, scale, scale);
	D3DXMatrixScaling(&radiusMatrix, m_Radius, m_Radius, m_Radius);

	switch (m_FaceID) {
		case Top:    rot(Vec3<float>((float)D3DX_PI / 2.0f, 0.0f, 0.0f));  mov(Vec3<float>( 0.0f,  0.5f,  0.0f));  break;
		case Bottom: rot(Vec3<float>((float)D3DX_PI / 2.0f, 0.0f, 0.0f));  mov(Vec3<float>( 0.0f, -0.5f,  0.0f));  break;
		case Left:   rot(Vec3<float>(0.0f, (float)D3DX_PI / 2.0f, 0.0f));  mov(Vec3<float>(-0.5f,  0.0f,  0.0f));  break;
		case Right:  rot(Vec3<float>(0.0f, (float)D3DX_PI / 2.0f, 0.0f));  mov(Vec3<float>( 0.5f,  0.0f,  0.0f));  break;
		case Front:  rot(Vec3<float>(0.0f, 0.0f, 0.0f));				   mov(Vec3<float>( 0.0f,  0.0f, -0.5f));  break;
		case Back:   rot(Vec3<float>(0.0f, 0.0f, 0.0f));				   mov(Vec3<float>( 0.0f,  0.0f,  0.5f));  break;
	}

	m_WorldMatrix = m_MatrixMov * m_ScaleMatrix;
	localMatrix	  = m_LRZ * m_LRX * m_LRY * m_LMovMatrix * sphereScale;

	for (auto &v : vertices) {
		v.position  = vectorTransform(mapPointToSphere(vectorTransform(v.position, localMatrix)), radiusMatrix);
		D3DXVec3Normalize(&v.normal, &v.position);
		v.position += v.normal * m_Terrain->getHeight(Vec2<float>(v.position.x, v.position.y));
	}

	for (int i = 0; i < m_IndexCount - 2; i++) {
		D3DXVECTOR3 p1 = vertices[indices[i]].position;
		D3DXVECTOR3 p2 = vertices[indices[i + 1]].position;
		D3DXVECTOR3 p3 = vertices[indices[i + 2]].position;

		D3DXVECTOR3 u = p2 - p1, v = p3 - p1;
		D3DXVECTOR3 n;

		n.x = (u.y * v.z) - (u.z * v.y);
		n.y = (u.z * v.x) - (u.x * v.z);
		n.z = (u.x * v.y) - (u.y * v.x);

		D3DXVec3Normalize(&vertices[indices[i]].normal, &n);

		float dot = D3DXVec3Dot(&n, &m_Terrain->getCamera()->getDxPosition());

		if (dot < 0)
			vertices[indices[i]].normal *= -1;
	}
	
	vertices[indices[m_IndexCount - 1]].normal = vertices[indices[m_IndexCount - 3]].normal;
	vertices[indices[m_IndexCount - 2]].normal = vertices[indices[m_IndexCount - 3]].normal;
	
	Vertex halfVertex = vertices[(m_GridSize * m_GridSize) / 2 - m_GridSize / 2];

	m_HalfPos = Vec3<float>(halfVertex.position.x, halfVertex.position.y, halfVertex.position.z);
	m_Normal  = Vec3<float>(halfVertex.normal.x  , halfVertex.normal.y  , halfVertex.normal.z  );

	m_Diameter = D3DXVec3Length(&(vertices[0].position - vertices[m_GridSize - 1].position));

	return initData(device, vertices, indices);
}

void TerrainPatch::cleanup() {
	Primitive::cleanup();
}

D3DXVECTOR3 TerrainPatch::mapPointToSphere(D3DXVECTOR3 p) {
	float x2 = p.x * p.x, y2 = p.y * p.y, z2 = p.z * p.z;

	return D3DXVECTOR3(p.x * sqrtf(1.0f - y2 * 0.5f - z2 * 0.5f + (y2 * z2) * 0.33333f),
		p.y * sqrtf(1.0f - z2 * 0.5f - x2 * 0.5f + (z2 * x2) * 0.33333f),
		p.z * sqrtf(1.0f - x2 * 0.5f - y2 * 0.5f + (x2 * y2) * 0.33333f));
}

void TerrainPatch::rot(Vec3<float> r) {
	D3DXMatrixRotationX(&m_LRX, r.x);
	D3DXMatrixRotationY(&m_LRY, r.y);
	D3DXMatrixRotationZ(&m_LRZ, r.z);
}

void TerrainPatch::mov(Vec3<float> m) {
	D3DXMatrixTranslation(&m_LMovMatrix, m.x, m.y, m.z);
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

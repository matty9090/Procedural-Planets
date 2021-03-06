#include "TerrainPatch.hpp"
#include "TerrainNode.hpp"

#include <DirectXMath.h>
#include <iostream>

using namespace DirectX;

TerrainPatch::TerrainPatch(Terrain *terrain, TerrainNode* node, int face, Rect bounds, float radius)
	: m_Terrain(terrain), m_FaceID(face), m_GridSize(17), m_Bounds(bounds), m_Radius(radius), m_Node(node)
{
	//for (int i = 0; i < m_GridSize * 8; ++i)
	//	m_Markers.push_back(new Primitive());
}

bool TerrainPatch::init(ID3D11Device *device, Shader *shader) {
	Primitive::init(device, shader);

	Vec3<float> colour(0, 0, 1);

	//for (size_t i = 0; i < m_Markers.size(); ++i) {
	//	m_Markers[i]->init(device, shader);
	//	m_Markers[i]->setColour(colour);
	//}

	m_Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float scale = 2.0f;
	m_Edges.resize(4);

	std::vector<Vertex> vertices;
	std::vector<unsigned long> indices;

	createPlane(vertices, indices);
	
	D3DXMATRIX sphereScale, localMatrix, radiusMatrix;

	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixTranslation(&m_MatrixMov, 0, 0, 0);

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

	fixDetailDifference(vertices, indices);
	calculateNormals(vertices, indices);
	
	Vertex halfVertex = vertices[(m_GridSize * m_GridSize) / 2 - m_GridSize / 2];

	m_HalfPos = Vec3<float>(halfVertex.position.x, halfVertex.position.y, halfVertex.position.z);
	m_Normal  = Vec3<float>(halfVertex.normal.x  , halfVertex.normal.y  , halfVertex.normal.z  );

	m_Diameter = D3DXVec3Length(&(vertices[0].position - vertices[m_GridSize - 1].position));

	return initData(device, vertices, indices);
}

void TerrainPatch::cleanup() {
	Primitive::cleanup();

	for (size_t i = 0; i < m_Markers.size(); ++i) {
		m_Markers[i]->cleanup();
		delete m_Markers[i];
	}
}

D3DXVECTOR3 TerrainPatch::mapPointToSphere(D3DXVECTOR3 p) {
	float x2 = p.x * p.x, y2 = p.y * p.y, z2 = p.z * p.z;

	return D3DXVECTOR3(p.x * sqrtf(1.0f - y2 * 0.5f - z2 * 0.5f + (y2 * z2) * 0.33333f),
		p.y * sqrtf(1.0f - z2 * 0.5f - x2 * 0.5f + (z2 * x2) * 0.33333f),
		p.z * sqrtf(1.0f - x2 * 0.5f - y2 * 0.5f + (x2 * y2) * 0.33333f));
}

void TerrainPatch::averageEdges(std::vector<Vertex> &vertices, std::vector<int> &points, int start, int end, int diff, int &count) {	
	int half	= (end - start) / 2;
	int edge	= points[start + half];
	int s_edge	= points[start];
	int e_edge	= points[end];

	vertices[edge].position = (vertices[s_edge].position + vertices[e_edge].position) / 2;
	vertices[edge].normal	= (vertices[s_edge].normal   + vertices[e_edge].normal  ) / 2;

	//m_Markers[count++]->setPosition(vertices[edge].position);

	if(diff > m_GridSize)
		m_Markers[count - 1]->setColour(Vec3<float>(1, 0, 0));

	if (half > 1) {
		averageEdges(vertices, points, start, start + half, diff, count);
		averageEdges(vertices, points, start + half, end, diff, count);
	}
}

void TerrainPatch::fixDetailDifference(std::vector<Vertex> &vertices, std::vector<unsigned long> &indices) {
	int edgeNum = 0;
	int count = 0;
	
	for (auto &edgeList : m_Edges) {
		if (m_Node->getNeighbour(edgeNum) && m_Node->getMaxDepth() > m_Node->getNeighbour(edgeNum)->getDepth()) {
			int diff = m_Node->getMaxDepth() - m_Node->getNeighbour(edgeNum)->getDepth();
			int num	= edgeList.size();

			diff = (int)pow(2, diff);
			
			/* Split neighbour if larger than current node's tree */
			if (diff > m_GridSize) {
				//std::cout << "Neighbour detail > quadtree size (" << (m_Node->getDepth() - m_Node->getNeighbour(edgeNum)->getDepth()) << ")\n";
				//m_Node->getNeighbour(edgeNum)->split();
			}

			/*for (int index = 0; index < num; index++) {
				vertices.erase(vertices.begin() + edgeList[index]);
				indices.erase(indices.begin() + edgeList[index]);
			}*/
			
			for (int index = 0; index < num - diff; index += diff)
				averageEdges(vertices, edgeList, index, index + diff, diff, count);
		} else {

		}

		++edgeNum;
	}
}

void TerrainPatch::createPlane(std::vector<Vertex> &vertices, std::vector<unsigned long> &indices) {
	float size_x = m_Bounds.x2 - m_Bounds.x;
	float size_y = m_Bounds.y2 - m_Bounds.y;

	float step_x = size_x / (m_GridSize - 1);
	float step_y = size_y / (m_GridSize - 1);

	m_Scale = sqrtf(size_x * size_x + size_y * size_y);

	for (int i = 0; i < m_GridSize; i++) {
		float y = i * step_y + m_Bounds.y;

		for (int j = 0; j < m_GridSize; j++) {
			float x = j * step_x + m_Bounds.x;

			D3DXVECTOR4 colour = D3DXVECTOR4(0.0f, 0.8f, 0.0f, 1.0f);
			vertices.push_back({ D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0, 0, 1), colour });

			if (i == 0) m_Edges[EdgeTop].push_back(vertices.size() - 1);
			if (j == 0) m_Edges[EdgeLeft].push_back(vertices.size() - 1);
			if (i == m_GridSize - 1) m_Edges[EdgeBottom].push_back(vertices.size() - 1);
			if (j == m_GridSize - 1) m_Edges[EdgeRight].push_back(vertices.size() - 1);
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
	m_IndexCount = indices.size();
}

void TerrainPatch::calculateNormals(std::vector<Vertex>& vertices, std::vector<unsigned long>& indices) {
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

#include "Primitive.hpp"

Primitive::Primitive() {
	m_VertexBuffer = NULL;
	m_VertexBuffer = NULL;
}

Primitive::~Primitive() {

}

bool Primitive::init(ID3D11Device *device) {
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

void Primitive::render(ID3D11DeviceContext *deviceContext) {
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Vertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Primitive::cleanup() {
	if (m_IndexBuffer)  m_IndexBuffer->Release();
	if (m_VertexBuffer) m_VertexBuffer->Release();
}

void Primitive::move(Vec3<float> p) {
	m_Pos += p;

	D3DXMatrixTranslation(&m_MatrixMov, m_Pos.x, m_Pos.y, m_Pos.z);

	m_WorldMatrix = m_MatrixMov;
}

void Primitive::rotate(Vec3<float> r) {
	D3DXMATRIX rotX, rotY, rotZ;
	m_Rot += r;

	D3DXMatrixRotationX(&rotX, m_Rot.x);
	D3DXMatrixRotationY(&rotY, m_Rot.y);
	D3DXMatrixRotationZ(&rotZ, m_Rot.z);

	m_WorldMatrix = m_MatrixMov * rotX * rotY * rotZ;
}

bool Primitive::initData(ID3D11Device *device, Vertex *vertices, unsigned long *indices) {
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer)))
		return false;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer)))
		return false;

	vertices = 0;
	indices = 0;

	return true;
}

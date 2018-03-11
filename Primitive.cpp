#include "Primitive.hpp"

Primitive::Primitive() : m_VertexCount(0), m_IndexCount(0) {
	m_VertexBuffer = NULL;
	m_IndexBuffer = NULL;
}

Primitive::~Primitive() {

}

bool Primitive::init(ID3D11Device * device, Shader * shader) {
	m_Device = device;
	m_Shader = shader;

	return true;
}

void Primitive::render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix) {
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Vertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_Shader->render(deviceContext, m_IndexCount, m_WorldMatrix, viewMatrix, projMatrix);
}

void Primitive::cleanup() {
	if (m_IndexBuffer)  m_IndexBuffer->Release();
	if (m_VertexBuffer) m_VertexBuffer->Release();
}

void Primitive::move(Vec3<float> p) {
	m_Pos += p;

	D3DXMatrixTranslation(&m_MatrixMov, m_Pos.x, m_Pos.y, m_Pos.z);

	m_WorldMatrix = m_ScaleMatrix * m_MatrixMov * m_RotZ * m_RotX * m_RotY;
}

void Primitive::rotate(Vec3<float> r) {
	m_Rot += r;

	D3DXMatrixRotationX(&m_RotX, m_Rot.x);
	D3DXMatrixRotationY(&m_RotY, m_Rot.y);
	D3DXMatrixRotationZ(&m_RotZ, m_Rot.z);

	m_WorldMatrix = m_ScaleMatrix * m_MatrixMov * m_RotZ * m_RotX * m_RotY;
}

bool Primitive::initData(ID3D11Device *device, std::vector<Vertex> &vertices, std::vector<unsigned long> &indices) {
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &vertices[0];
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

	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer)))
		return false;

	return true;
}

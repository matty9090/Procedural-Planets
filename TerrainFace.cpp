#include "TerrainFace.hpp"

TerrainFace::TerrainFace(int id) : m_FaceID(id), m_Node(new TerrainNode(NULL, { 0, 0, 1, 1 }, m_FaceID)) {

}

bool TerrainFace::init(ID3D11Device *device, Shader *shader) {
	m_Node->init(device, shader);

	return true;
}

void TerrainFace::render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix) {
	m_Node->render(deviceContext, viewMatrix, projMatrix);
}

void TerrainFace::cleanup() {
	delete m_Node;
}

TerrainFace::~TerrainFace() {
	
}

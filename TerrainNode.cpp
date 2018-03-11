#include "TerrainNode.hpp"

TerrainNode::TerrainNode(TerrainNode *parent, Rect bounds, int face)
	: m_Parent(parent), m_Patch(new TerrainPatch(face))
{

}

void TerrainNode::init(ID3D11Device *device, Shader *shader) {
	m_Patch->init(device, shader);
}

void TerrainNode::render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix) {
	m_Patch->render(deviceContext, viewMatrix, projMatrix);
}

TerrainNode::~TerrainNode() {

}
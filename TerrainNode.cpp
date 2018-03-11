#include "TerrainNode.hpp"

TerrainNode::TerrainNode(TerrainNode *parent, Rect bounds, int face)
	: m_Parent(parent), m_Patch(new TerrainPatch(face, bounds)), m_IsLeaf(true),
	  m_FaceID(face), m_Depth(parent ? parent->m_Depth + 1 : 0), m_Bounds(bounds)
{

}

void TerrainNode::init(ID3D11Device *device, Shader *shader) {
	m_Patch->init(device, shader);
}

void TerrainNode::render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix) {
	if(m_IsLeaf)
		m_Patch->render(deviceContext, viewMatrix, projMatrix);
	else {
		m_Top->render(deviceContext, viewMatrix, projMatrix);
		m_Right->render(deviceContext, viewMatrix, projMatrix);
		m_Bottom->render(deviceContext, viewMatrix, projMatrix);
		m_Left->render(deviceContext, viewMatrix, projMatrix);
	}
}

void TerrainNode::split() {
	if (m_Depth > 5)
		return;

	if (m_IsLeaf) {
		m_IsLeaf = false;

		Rect b = m_Bounds;

		float w = (b.x2 - b.x) / 2;
		float h = (b.y2 - b.y) / 2;

		m_Top    = new TerrainNode(this, { b.x    , b.y    , b.x2 - w, b.y2 - h }, m_FaceID);
		m_Right  = new TerrainNode(this, { b.x + w, b.y    , b.x2    , b.y2 - h }, m_FaceID);
		m_Bottom = new TerrainNode(this, { b.x + w, b.y + h, b.x2    , b.y2     }, m_FaceID);
		m_Left   = new TerrainNode(this, { b.x    , b.y + h, b.x2 - w, b.y2     }, m_FaceID);

		m_Top->init(m_Patch->getDevice(), m_Patch->getShader());
		m_Right->init(m_Patch->getDevice(), m_Patch->getShader());
		m_Bottom->init(m_Patch->getDevice(), m_Patch->getShader());
		m_Left->init(m_Patch->getDevice(), m_Patch->getShader());

		m_Patch->cleanup();
		delete m_Patch;
	} else {
		m_Top->split();
		m_Right->split();
		m_Bottom->split();
		m_Left->split();
	}
}

TerrainNode::~TerrainNode() {

}
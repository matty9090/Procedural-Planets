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

void TerrainNode::update(D3DXVECTOR3 cameraPos) {

}

void TerrainNode::cleanup() {
	m_Patch->cleanup();
	delete m_Patch;
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
	} else {
		m_Top->split();
		m_Right->split();
		m_Bottom->split();
		m_Left->split();
	}
}

void TerrainNode::merge() {
	if (m_IsLeaf)
		return;

	if (m_Top->isLeaf()) {
		m_IsLeaf = true;

		m_Top->cleanup();
		m_Right->cleanup();
		m_Bottom->cleanup();
		m_Left->cleanup();

		delete m_Top;
		delete m_Right;
		delete m_Bottom;
		delete m_Left;
	} else {
		m_Top->merge();
		m_Right->merge();
		m_Bottom->merge();
		m_Left->merge();
	}
}

TerrainNode::~TerrainNode() {

}
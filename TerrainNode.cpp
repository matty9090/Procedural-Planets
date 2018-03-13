#include "TerrainNode.hpp"

TerrainNode::TerrainNode(TerrainNode *parent, Rect bounds, int face, Camera *cam, float radius)
	: m_Parent(parent), m_Patch(new TerrainPatch(face, bounds, radius)), m_IsLeaf(true), m_Camera(cam),
	  m_FaceID(face), m_Depth(parent ? parent->m_Depth + 1 : 0), m_Bounds(bounds), m_Radius(radius)
{

}

void TerrainNode::init(ID3D11Device *device, Shader *shader) {
	m_Patch->init(device, shader);
}

void TerrainNode::render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix) {
	if(m_IsLeaf)
		m_Patch->render(deviceContext, viewMatrix, projMatrix);
	else {
		m_NW->render(deviceContext, viewMatrix, projMatrix);
		m_NE->render(deviceContext, viewMatrix, projMatrix);
		m_SE->render(deviceContext, viewMatrix, projMatrix);
		m_SW->render(deviceContext, viewMatrix, projMatrix);
	}
}

void TerrainNode::update() {
	float distance = m_Patch->getCenterPos().distance(m_Camera->getPosition()) / m_Radius;
	bool divide = distance < m_Patch->getDiameter() * 2.0f;

	if (!divide)
		merge();

	if (m_IsLeaf) {
		if (divide)
			split();
	} else if (!m_IsLeaf) {
		m_NW->update(), m_NE->update();
		m_SE->update(), m_SW->update();
	}
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

		m_NW = new TerrainNode(this, { b.x    , b.y    , b.x2 - w, b.y2 - h }, m_FaceID, m_Camera, m_Radius);
		m_NE = new TerrainNode(this, { b.x + w, b.y    , b.x2    , b.y2 - h }, m_FaceID, m_Camera, m_Radius);
		m_SE = new TerrainNode(this, { b.x + w, b.y + h, b.x2    , b.y2     }, m_FaceID, m_Camera, m_Radius);
		m_SW = new TerrainNode(this, { b.x    , b.y + h, b.x2 - w, b.y2     }, m_FaceID, m_Camera, m_Radius);

		m_NW->init(m_Patch->getDevice(), m_Patch->getShader());
		m_NE->init(m_Patch->getDevice(), m_Patch->getShader());
		m_SE->init(m_Patch->getDevice(), m_Patch->getShader());
		m_SW->init(m_Patch->getDevice(), m_Patch->getShader());
	} else {
		m_NW->split(), m_NE->split();
		m_SE->split(), m_SW->split();
	}
}

void TerrainNode::merge() {
	if (m_IsLeaf)
		return;

	if (m_NW->isLeaf()) {
		m_IsLeaf = true;

		m_NW->cleanup(), m_NE->cleanup();
		m_SE->cleanup(), m_SW->cleanup();

		delete m_NW, delete m_NE;
		delete m_SE, delete m_SW;
	} else {
		m_NW->merge(), m_NE->merge();
		m_SE->merge(), m_SW->merge();
	}
}

TerrainNode::~TerrainNode() {

}
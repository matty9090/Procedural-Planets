#include "TerrainNode.hpp"

#include <algorithm>

TerrainNode::TerrainNode(Terrain *terrain, TerrainNode *parent, Rect bounds, int face, Camera *cam, float radius)
	: m_Parent(parent), m_Patch(new TerrainPatch(terrain, this, face, bounds, radius)), m_IsLeaf(true), m_Camera(cam), m_IsVisible(true),
	  m_FaceID(face), m_Depth(parent ? parent->m_Depth + 1 : 0), m_Bounds(bounds), m_Radius(radius), m_Terrain(terrain)
{

}

void TerrainNode::init(ID3D11Device *device, Shader *shader) {
	m_Patch->init(device, shader);
}

void TerrainNode::render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXVECTOR3 camPos, D3DXVECTOR3 lightPos, D3DXVECTOR3 lightCol, D3DXVECTOR3 ambientColour) {
	if (m_IsVisible) {
		if (m_IsLeaf) {
			m_Patch->render(deviceContext, viewMatrix, projMatrix, camPos, lightPos, lightCol, ambientColour);

			//for (auto &marker : m_Patch->getMarkers())
			//	marker->render(deviceContext, viewMatrix, projMatrix, camPos, lightPos, lightCol, ambientColour);
		} else {
			m_NW->render(deviceContext, viewMatrix, projMatrix, camPos, lightPos, lightCol, ambientColour);
			m_NE->render(deviceContext, viewMatrix, projMatrix, camPos, lightPos, lightCol, ambientColour);
			m_SE->render(deviceContext, viewMatrix, projMatrix, camPos, lightPos, lightCol, ambientColour);
			m_SW->render(deviceContext, viewMatrix, projMatrix, camPos, lightPos, lightCol, ambientColour);
		}
	}
}

void TerrainNode::connect(TerrainNode *north, TerrainNode *east, TerrainNode *south, TerrainNode *west) {
	m_NorthNhbr = north;
	m_EastNhbr	= east;
	m_SouthNhbr = south;
	m_WestNhbr	= west;
}

void TerrainNode::update() {
	float height = m_Camera->getPosition().length() - m_Radius;
	float dist = m_Camera->getPosition().distance(m_Patch->getCenterPos()) - m_Patch->getDiameter();
	
	float horizon = sqrtf(height * (2 * m_Radius + height));

	m_IsVisible = dist < horizon;
	
	if (m_IsVisible) {
		float distance = m_Patch->getCenterPos().distance(m_Camera->getPosition()) / m_Radius;
		bool divide = distance < m_Patch->getScale() * 2.2f;

		if (!divide)
			merge();

		if (m_IsLeaf && divide) {
			split();
		} else if (!m_IsLeaf) {
			m_NW->update(), m_NE->update();
			m_SE->update(), m_SW->update();
		}
	}
}

void TerrainNode::cleanup() {
	m_Patch->cleanup();
	delete m_Patch;
}

void TerrainNode::split() {
	if (m_Depth > 15)
		return;	

	if (m_IsLeaf) {
		m_IsLeaf = false;

		Rect b = m_Bounds;

		float w = (b.x2 - b.x) / 2;
		float h = (b.y2 - b.y) / 2;

		m_NW = new TerrainNode(m_Terrain, this, { b.x    , b.y    , b.x2 - w, b.y2 - h }, m_FaceID, m_Camera, m_Radius);
		m_NE = new TerrainNode(m_Terrain, this, { b.x + w, b.y    , b.x2    , b.y2 - h }, m_FaceID, m_Camera, m_Radius);
		m_SE = new TerrainNode(m_Terrain, this, { b.x + w, b.y + h, b.x2    , b.y2     }, m_FaceID, m_Camera, m_Radius);
		m_SW = new TerrainNode(m_Terrain, this, { b.x    , b.y + h, b.x2 - w, b.y2     }, m_FaceID, m_Camera, m_Radius);

		m_NW->connect(m_NorthNhbr, m_NE, m_SW, m_WestNhbr), m_NE->connect(m_NorthNhbr, m_EastNhbr, m_SE, m_NW);
		m_SE->connect(m_NE, m_EastNhbr, m_SouthNhbr, m_SW), m_SW->connect(m_NW, m_SE, m_SouthNhbr, m_WestNhbr);

		m_NW->init(m_Patch->getDevice(), m_Patch->getShader()), m_NE->init(m_Patch->getDevice(), m_Patch->getShader());
		m_SE->init(m_Patch->getDevice(), m_Patch->getShader()), m_SW->init(m_Patch->getDevice(), m_Patch->getShader());
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

void TerrainNode::rebuild() {
	m_Patch->getMarkers().clear();
	m_Patch->cleanup();
	delete m_Patch;

	m_Patch = new TerrainPatch(m_Terrain, this, m_FaceID, m_Bounds, m_Radius);
	m_Patch->init(m_Terrain->getGraphics()->getDevice(), m_Terrain->getShader());
}

void TerrainNode::rebuildNeighbours() {
	if (m_NorthNhbr) m_NorthNhbr->rebuild();
	if (m_EastNhbr) m_EastNhbr->rebuild();
	if (m_SouthNhbr) m_SouthNhbr->rebuild();
	if (m_WestNhbr) m_WestNhbr->rebuild();
}

int TerrainNode::getMaxDepth() {
	if (!m_IsLeaf) {
		int d1 = m_NE->getMaxDepth();
		int d2 = m_NW->getMaxDepth();
		int d3 = m_SE->getMaxDepth();
		int d4 = m_SW->getMaxDepth();

		return (std::max)({ d1, d2, d3, d4 }) + 1;
	}

	return m_Depth;
}

TerrainNode *TerrainNode::getNeighbour(int n) {
	switch (n) {
		case TerrainPatch::EdgeTop: return m_NorthNhbr; break;
		case TerrainPatch::EdgeRight: return m_EastNhbr; break;
		case TerrainPatch::EdgeBottom: return m_SouthNhbr; break;
		case TerrainPatch::EdgeLeft: return m_WestNhbr; break;
	}

	return nullptr;
}

TerrainNode::~TerrainNode() {

}
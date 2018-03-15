#include "TerrainFace.hpp"

#include "Input.hpp"
#include "Terrain.hpp"

TerrainFace::TerrainFace(Terrain *terrain, int id, Camera *cam, float radius) : m_FaceID(id), m_Camera(cam),
	m_Node(new TerrainNode(terrain, NULL, { -0.5f, -0.5f, 0.5f, 0.5f }, m_FaceID, cam, radius)) {

}

bool TerrainFace::init(ID3D11Device *device, Shader *shader) {
	m_Node->init(device, shader);

	return true;
}

void TerrainFace::render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXVECTOR3 camPos, D3DXVECTOR3 lightPos, D3DXVECTOR3 lightCol, D3DXVECTOR3 ambientColour) {
	m_Node->render(deviceContext, viewMatrix, projMatrix, camPos, lightPos, lightCol, ambientColour);
}

void TerrainFace::update() {
	m_Node->update();
}

void TerrainFace::split() {
	m_Node->split();
}

void TerrainFace::merge() {
	m_Node->merge();
}

void TerrainFace::cleanup() {
	delete m_Node;
}

TerrainFace::~TerrainFace() {
	
}

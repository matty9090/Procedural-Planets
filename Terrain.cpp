#include "Terrain.hpp"
#include "Input.hpp"

Terrain::Terrain(ID3D11Device *device, ID3D11DeviceContext *deviceContext, Shader *shader, Camera *cam)
	: m_Device(device), m_DeviceContext(deviceContext), m_Shader(shader), m_Camera(cam) {
	
}

Terrain::~Terrain() {

}

bool Terrain::init() {
	faces.push_back(new TerrainFace(TerrainFace::Top,    m_Camera));
	faces.push_back(new TerrainFace(TerrainFace::Bottom, m_Camera));
	faces.push_back(new TerrainFace(TerrainFace::Left,   m_Camera));
	faces.push_back(new TerrainFace(TerrainFace::Right,  m_Camera));
	faces.push_back(new TerrainFace(TerrainFace::Front,  m_Camera));
	faces.push_back(new TerrainFace(TerrainFace::Back,   m_Camera));

	for (auto &face : faces)
		face->init(m_Device, m_Shader);

	return true;
}

void Terrain::render(D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix) {
	if (Input::KeyHit(Input::Key_F2)) {
		for (auto &face : faces)
			face->split();
	}

	if (Input::KeyHit(Input::Key_F1)) {
		for (auto &face : faces)
			face->merge();
	}

	for (auto &face : faces)
		face->render(m_DeviceContext, viewMatrix, projMatrix);
}

void Terrain::update() {
	for (auto &face : faces)
		face->update();
}

void Terrain::cleanup() {
	for (auto &face : faces)
		face->cleanup();
}
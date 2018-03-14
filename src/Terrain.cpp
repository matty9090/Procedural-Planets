#include "Terrain.hpp"
#include "Input.hpp"
#include "TerrainFace.hpp"

Terrain::Terrain(ID3D11Device *device, ID3D11DeviceContext *deviceContext, Shader *shader, Camera *cam, float radius)
	: m_Device(device), m_DeviceContext(deviceContext), m_Shader(shader), m_Camera(cam), m_Radius(radius) {
	
	m_Noise = new SimplexNoise(1.0f, 1.0f, 2.0f, 0.5f);
}

Terrain::~Terrain() {

}

bool Terrain::init() {
	faces.push_back(new TerrainFace(this, TerrainFace::Top,    m_Camera, m_Radius));
	faces.push_back(new TerrainFace(this, TerrainFace::Bottom, m_Camera, m_Radius));
	faces.push_back(new TerrainFace(this, TerrainFace::Left,   m_Camera, m_Radius));
	faces.push_back(new TerrainFace(this, TerrainFace::Right,  m_Camera, m_Radius));
	faces.push_back(new TerrainFace(this, TerrainFace::Front,  m_Camera, m_Radius));
	faces.push_back(new TerrainFace(this, TerrainFace::Back,   m_Camera, m_Radius));

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

float Terrain::getHeight(Vec2<float> pos) {
	return m_Noise->fractal(5, pos.x * 0.1f, pos.y * 0.1f);
}

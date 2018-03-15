#include "Terrain.hpp"
#include "Input.hpp"
#include "TerrainFace.hpp"

Terrain::Terrain(ID3D11Device *device, ID3D11DeviceContext *deviceContext, Graphics *graphics, Shader *shader, Camera *cam, float radius)
	: m_Device(device), m_DeviceContext(deviceContext), m_Shader(shader), m_Camera(cam), m_Radius(radius), m_Graphics(graphics) {
	
	m_Noise = new SimplexNoise(1.0f, 1.8f, 2.2f, 0.5f);
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

void Terrain::render(D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXVECTOR3 camPos, D3DXVECTOR3 lightPos, D3DXVECTOR3 lightCol, D3DXVECTOR3 ambientColour) {
	if (Input::KeyHit(Input::Key_F2)) {
		for (auto &face : faces)
			face->split();
	}

	if (Input::KeyHit(Input::Key_F1)) {
		for (auto &face : faces)
			face->merge();
	}

	for (auto &face : faces)
		face->render(m_DeviceContext, viewMatrix, projMatrix, camPos, lightPos, lightCol, ambientColour);

	float height = m_Camera->getPosition().length() - m_Radius;
	float horizon = sqrtf(height * (2 * m_Radius + height));

	m_Graphics->drawTextValue(10, 40, "Horizon Dist", horizon);
	m_Graphics->drawTextValue(10, 70, "Height", height);
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
	return m_Noise->fractal(9, pos.x * 0.05f, pos.y * 0.05f);
}

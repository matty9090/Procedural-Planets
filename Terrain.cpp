#include "Terrain.hpp"

Terrain::Terrain(ID3D11Device *device, ID3D11DeviceContext *deviceContext, Shader *shader)
	: m_Device(device), m_DeviceContext(deviceContext), m_Shader(shader) {
	
}

Terrain::~Terrain() {

}

bool Terrain::init() {
	faces.push_back(new TerrainFace(TerrainFace::Top));
	faces.push_back(new TerrainFace(TerrainFace::Bottom));
	faces.push_back(new TerrainFace(TerrainFace::Left));
	faces.push_back(new TerrainFace(TerrainFace::Right));
	faces.push_back(new TerrainFace(TerrainFace::Front));
	faces.push_back(new TerrainFace(TerrainFace::Back));

	for (auto &face : faces)
		face->init(m_Device, m_Shader);

	return true;
}

void Terrain::render(D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix) {
	for (auto &face : faces) {
		//face->rotate(Vec3<>(0, 0.00018f, 0));
		face->render(m_DeviceContext, viewMatrix, projMatrix);
	}
}

void Terrain::cleanup() {
	for (auto &face : faces)
		face->cleanup();
}
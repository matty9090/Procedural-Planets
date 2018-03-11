#pragma once

#include "TerrainPatch.hpp"

struct Rect {
	float x, y;
	float x2, y2;

	bool contains(float i, float j) {
		return (i <= x2 && i >= x && j <= y2 && j >= y);
	}
};

class TerrainNode {
	public:
		TerrainNode(TerrainNode *parent, Rect bounds, int face);
		~TerrainNode();

		void init(ID3D11Device *device, Shader *shader);
		void render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);

	private:
		Rect m_Bounds;
		TerrainNode *m_Parent;
		TerrainNode *m_Top, *m_Right, *m_Bottom, *m_Left;
		TerrainPatch *m_Patch;
};

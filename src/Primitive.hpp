#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>

#include "Vec3.hpp"
#include "Shader.hpp"

struct Rect {
	float x, y;
	float x2, y2;

	bool contains(float i, float j) {
		return (i <= x2 && i >= x && j <= y2 && j >= y);
	}
};

class Primitive {
	public:
		Primitive();
		~Primitive();

		virtual bool init(ID3D11Device *device, Shader *shader);

		void render(ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix, D3DXVECTOR3 camPos, D3DXVECTOR3 lightPos, D3DXVECTOR3 lightCol, D3DXVECTOR3 ambientColour);
		void cleanup();

		void move(Vec3<float> p);
		void rotate(Vec3<float> r);

		int getIndexCount() { return m_IndexCount; }

		D3DMATRIX getWorldMatrix() { return m_WorldMatrix; }
		Shader *getShader() { return m_Shader; }
		ID3D11Device *getDevice() { return m_Device; }

	protected:
		ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
		ID3D11Device *m_Device;

		D3DXMATRIX m_WorldMatrix, m_MatrixMov;
		D3DXMATRIX m_RotX, m_RotY, m_RotZ;
		D3DXMATRIX m_ScaleMatrix;

		Vec3<float> m_Pos, m_Rot;
		Shader *m_Shader;

		int m_VertexCount, m_IndexCount;

		struct Vertex {
			D3DXVECTOR3 position;
			D3DXVECTOR3 normal;
			D3DXVECTOR4 color;
		};

		bool initData(ID3D11Device *device, std::vector<Vertex> &vertices, std::vector<unsigned long> &indices);
};


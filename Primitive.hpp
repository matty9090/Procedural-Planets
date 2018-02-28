#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#include "Vec3.hpp"

class Primitive {
	public:
		Primitive();
		~Primitive();

		virtual bool init(ID3D11Device *device);
		void render(ID3D11DeviceContext *deviceContext);
		void cleanup();

		void move(Vec3<float> p);
		void rotate(Vec3<float> r);

		int getIndexCount() { return m_IndexCount; }

		D3DMATRIX getWorldMatrix() { return m_WorldMatrix; }

	protected:
		ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
		D3DXMATRIX m_WorldMatrix, m_MatrixMov;

		Vec3<float> m_Pos, m_Rot;

		int m_VertexCount, m_IndexCount;

		struct Vertex {
			D3DXVECTOR3 position;
			D3DXVECTOR4 color;
		};

		bool initData(ID3D11Device *device, Vertex *vertices, unsigned long *indices);
};


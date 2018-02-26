#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

class Primitive {
	public:
		Primitive();
		~Primitive();

		bool init(ID3D11Device *device);
		void render(ID3D11DeviceContext *deviceContext);
		void cleanup();

		int getIndexCount() { return m_IndexCount; }

	private:
		ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
		int m_VertexCount, m_IndexCount;

		struct Vertex {
			D3DXVECTOR3 position;
			D3DXVECTOR4 color;
		};
};


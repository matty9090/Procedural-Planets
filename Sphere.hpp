#pragma once

#include "Primitive.hpp"

#include <D3dx9shape.h>

class Sphere : public Primitive {
	public:
		Sphere();
		~Sphere();

		bool init(ID3D11Device *device);
		void cleanVertices(Vertex *vertices, unsigned long *indices);

	private:

};

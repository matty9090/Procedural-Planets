#pragma once

#include "Primitive.hpp"

#include <D3dx9shape.h>
#include <vector>

class Sphere : public Primitive {
	public:
		Sphere();
		~Sphere();

		bool init(ID3D11Device *device);
		void cleanVertices(Vertex *vertices, unsigned long *indices);

	private:
		enum Faces { Top, Bottom, Left, Right, Front, Back };
		
		std::pair<std::vector<Vertex>, std::vector<int>> generateGrid(int face, size_t size);
};

#pragma once

#include "Primitive.hpp"

class Sphere : public Primitive {
	public:
		Sphere();
		~Sphere();

		bool init(ID3D11Device *device);

	private:

};

#pragma once

#include"..\Base\Pch.h"

namespace Begginer
{
	class Material
	{
	public: 
		Material():diffuseAlpha(0),specularity(0) {}
		~Material() {}

		aiColor3D diffuse, specular, ambient;
		float diffuseAlpha, specularity;
	private:
	};
}
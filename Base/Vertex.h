#pragma once

#include"Vector3.h"
#include"RGBA.h"
#include<vector>

namespace Beginner
{
	//3D���f���̒��_���
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 uv;
		unsigned short bone[2];
		unsigned char boneWeight;
		unsigned char edgeFlg;

		/*std::vector<Vector3> texcoord;
		std::vector<RGBA> color;
		Vector3 tangent,bitangent;*/
	};
}
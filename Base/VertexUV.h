#pragma once

#include<DirectXMath.h>

namespace Beginner
{
	struct VertexUV
	{
		VertexUV() :VertexUV({ NULL,NULL,NULL }, { NULL,NULL })
			{}
		VertexUV(const DirectX::XMFLOAT3 Vertex, const DirectX::XMFLOAT2 UV)
			:vertex(Vertex), uv(UV) {}
		VertexUV(const float x, const float y, const float z, const float u, const float v)
			:vertex(x, y, z), uv(u, v) {}

		~VertexUV() {}

		DirectX::XMFLOAT3 vertex;
		DirectX::XMFLOAT2 uv;
	};
}
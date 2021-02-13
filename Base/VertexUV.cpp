#include"VertexUV.h"

VertexUV::VertexUV()
	:VertexUV({ NULL,NULL,NULL }, { NULL,NULL })
{

}

VertexUV::VertexUV(const DirectX::XMFLOAT3 Vertex, const DirectX::XMFLOAT2 UV)
	:vertex(Vertex),uv(UV)
{

}

VertexUV::VertexUV(const float x, const float y, const float z, const float u, const float v)
	:vertex(x, y, z), uv(u, v)
{

}

VertexUV::~VertexUV()
{

}

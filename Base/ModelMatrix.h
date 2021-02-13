#pragma once

#include<DirectXMath.h>

namespace Begginer
{
	//3dモデル用HLSLに転送する行列データ
	struct ModelMatrix
	{
		DirectX::XMMATRIX world;//world行列
		DirectX::XMMATRIX worldViewProj;//world * view * proj
	};
}
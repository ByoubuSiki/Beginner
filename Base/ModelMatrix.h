#pragma once

#include<DirectXMath.h>

namespace Begginer
{
	//3d���f���pHLSL�ɓ]������s��f�[�^
	struct ModelMatrix
	{
		DirectX::XMMATRIX world;//world�s��
		DirectX::XMMATRIX worldViewProj;//world * view * proj
	};
}
#pragma once

#include"..\Base\Pch.h"

namespace Begginer
{
	struct Mesh
	{
		unsigned vertexIndex[3];//頂点インデックスの取得
		unsigned short matIndex;//マテリアルインデックス
	};
}
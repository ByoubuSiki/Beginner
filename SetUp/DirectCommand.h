#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	//GPUに対する命令を管理

		bool CreateAllocator();//コマンドアロケータの初期化
		bool CreateGraphicsList();//コマンドリストの初期化
		bool CreateQueue();//コマンドキューの作成

		bool CreateCommand();//メンバを初期化

}
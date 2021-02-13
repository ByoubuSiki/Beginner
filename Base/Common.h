#pragma once

#define NOMINMAX

#include<Windows.h>
#include<unordered_map>
#include<d3d12.h>
#include<wrl.h>

#include"struct.h"

namespace Beginner
{
	//共通変数

	class Eye;
	class Base;


	extern std::list<Base*> registBase;//描画対象オブジェクト
	extern Eye* mainEye;//視点
	extern Microsoft::WRL::ComPtr<ID3D12Device> device;//描画デバイス
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;//命令リスト
}
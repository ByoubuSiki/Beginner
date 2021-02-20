#pragma once

#include"..\Base\Pch.h"
#include"DescriptorHeap.h"

namespace Beginner
{

	bool CreateDevice();//デバイスの初期化
	bool CreateDXGIFactory();//DXGIの管理オブジェクトを初期化
	bool GetUseAdapter();//使用できるアダプタの取得

	bool CreateSwapChain();//スワップチェインの作成

	UINT GetBackBufferSize();//バックバッファ数を返す
	//RTVとの関連付け
	bool CreateBackBuffer(std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>&, D3D12_CPU_DESCRIPTOR_HANDLE);

	bool CreateUntilAdapter();//アダプタの取得までの初期化

	bool CreateUntilEnd(//残った初期化を行う
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>&,
		const D3D12_CPU_DESCRIPTOR_HANDLE
	);
}
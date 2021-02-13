#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	class DescriptorHeap
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;//ヒータデータ

	public:
		DescriptorHeap() :heap(nullptr) {}
		~DescriptorHeap() {}
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetHeap();

		bool CreateRTV(const Microsoft::WRL::ComPtr<ID3D12Device>&, const USHORT);//レンダーターゲットの作成
		bool CreateCBV_SRV_UAV(const Microsoft::WRL::ComPtr<ID3D12Device>&, const USHORT);//定数リソースの作成
		bool CreateDSV(const Microsoft::WRL::ComPtr<ID3D12Device>&);//DSV作製
	};
}
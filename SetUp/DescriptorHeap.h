#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	class DescriptorHeap
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;//�q�[�^�f�[�^

	public:
		DescriptorHeap() :heap(nullptr) {}
		~DescriptorHeap() {}
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetHeap();

		bool CreateRTV(const Microsoft::WRL::ComPtr<ID3D12Device>&, const USHORT);//�����_�[�^�[�Q�b�g�̍쐬
		bool CreateCBV_SRV_UAV(const Microsoft::WRL::ComPtr<ID3D12Device>&, const USHORT);//�萔���\�[�X�̍쐬
		bool CreateDSV(const Microsoft::WRL::ComPtr<ID3D12Device>&);//DSV�쐻
	};
}
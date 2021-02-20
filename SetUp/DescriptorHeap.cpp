#include"DescriptorHeap.h"

namespace Beginner
{

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeap::GetHeap()
	{
		return heap;
	}

	//SRV��CBV��ׂ荇�킹�ō쐬
	bool DescriptorHeap::CreateCBV_SRV_UAV(const USHORT num)
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeap = {};
		{
			descHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			descHeap.NodeMask = 0;
			descHeap.NumDescriptors = num;
			descHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		}

		HRESULT result = device->CreateDescriptorHeap(
			&descHeap, IID_PPV_ARGS(heap.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("CBV_SRV_UAV��DescriptorHeap�̍쐬�����s\n");
			return false;
		}

		return true;
	}

	//RTV�̍쐬
	bool DescriptorHeap::CreateRTV(const USHORT num)
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};//�q�[�v�̐ݒ�
		{
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//�����_�[�^�[�Q�b�g
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;//�t���O����
			rtvHeapDesc.NodeMask = 0;
			rtvHeapDesc.NumDescriptors = num;
		}

		HRESULT result = device->CreateDescriptorHeap(
			&rtvHeapDesc, IID_PPV_ARGS(heap.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("RTV�p��DescriptorHeap�̍쐬���s\n");
			return false;
		}

		return true;
	}

	bool DescriptorHeap::CreateDSV()
	{
		D3D12_DESCRIPTOR_HEAP_DESC depthDesc = {};
		{
			depthDesc.NumDescriptors = 1;
			depthDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		}

		HRESULT result = device->CreateDescriptorHeap(&depthDesc, IID_PPV_ARGS(heap.ReleaseAndGetAddressOf()));

		if (FAILED(result))
		{
			DebugLogOnConsole("DSV�̍쐻�����s\n");
			return false;
		}

		return true;
	}
}
#include"DescriptorHeap.h"

namespace Beginner
{

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeap::GetHeap()
	{
		return heap;
	}

	//SRVとCBVを隣り合わせで作成
	bool DescriptorHeap::CreateCBV_SRV_UAV(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const USHORT num)
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
			DebugLogOnConsole("CBV_SRV_UAVのDescriptorHeapの作成が失敗\n");
			return false;
		}

		return true;
	}

	//RTVの作成
	bool DescriptorHeap::CreateRTV(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const USHORT num)
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};//ヒープの設定
		{
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲット
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;//フラグ無し
			rtvHeapDesc.NodeMask = 0;
			rtvHeapDesc.NumDescriptors = num;
		}

		HRESULT result = device->CreateDescriptorHeap(
			&rtvHeapDesc, IID_PPV_ARGS(heap.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("RTV用のDescriptorHeapの作成失敗\n");
			return false;
		}

		return true;
	}

	bool DescriptorHeap::CreateDSV(const Microsoft::WRL::ComPtr<ID3D12Device>& device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC depthDesc = {};
		{
			depthDesc.NumDescriptors = 1;
			depthDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		}

		HRESULT result = device->CreateDescriptorHeap(&depthDesc, IID_PPV_ARGS(heap.ReleaseAndGetAddressOf()));

		if (FAILED(result))
		{
			DebugLogOnConsole("DSVの作製が失敗\n");
			return false;
		}

		return true;
	}
}
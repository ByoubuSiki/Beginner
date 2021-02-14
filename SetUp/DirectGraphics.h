#pragma once

#include"..\Base\Pch.h"
#include"DescriptorHeap.h"

namespace Beginner
{

	//DirectXの描画を管理
	class DirectGraphics
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Device> device;
		Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> useAdapter;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

		bool CreateDevice();//デバイスの初期化
		bool CreateDXGIFactory();//DXGIの管理オブジェクトを初期化
		bool GetUseAdapter();//使用できるアダプタの取得

		bool CreateSwapChain(const Microsoft::WRL::ComPtr<ID3D12CommandQueue>&);//スワップチェインの作成

		UINT GetBackBufferSize();//バックバッファ数を返す
		//RTVとの関連付け
		bool CreateBackBuffer(std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>&, D3D12_CPU_DESCRIPTOR_HANDLE);

	public:
		DirectGraphics() :device(nullptr),dxgiFactory(nullptr),useAdapter(nullptr),swapChain(nullptr){}
		~DirectGraphics() {}

		bool CreateUntilAdapter();//アダプタの取得までの初期化

		bool CreateUntilEnd(//残った初期化を行う
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>&,
			std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>&,
			const D3D12_CPU_DESCRIPTOR_HANDLE
		);

		Microsoft::WRL::ComPtr<ID3D12Device> GetDevice();
		Microsoft::WRL::ComPtr<IDXGIFactory6> GetDxgiFactory();
		Microsoft::WRL::ComPtr<IDXGIAdapter> GetAdapter();
		Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain();
	};

}
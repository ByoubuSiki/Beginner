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

		bool CreateSwapChain(//スワップチェインの作成
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>&, const HWND
		);

		UINT GetBackBufferSize();//バックバッファ数を返す
		//RTVとの関連付け
		bool CreateBackBuffer(std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>&, D3D12_CPU_DESCRIPTOR_HANDLE);

	public:
		DirectGraphics();
		~DirectGraphics();

		bool CreateUntilAdapter();//アダプタの取得までの初期化

		bool CreateUntilEnd(//残った初期化を行う
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>&,
			std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>&,
			const D3D12_CPU_DESCRIPTOR_HANDLE, const HWND
		);

		Microsoft::WRL::ComPtr<ID3D12Device> GetDevice();
		Microsoft::WRL::ComPtr<IDXGIFactory6> GetDxgiFactory();
		Microsoft::WRL::ComPtr<IDXGIAdapter> GetAdapter();
		Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain();
	};

}
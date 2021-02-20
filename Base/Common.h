#pragma once

#define NOMINMAX

#include<Windows.h>
#include<unordered_map>
#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_6.h>

#include"struct.h"

namespace Beginner
{
	//共通変数

	class Eye;
	class Base;

	extern HWND beginHwnd;
	extern std::list<Base*> outputObject;//描画対象オブジェクト
	extern Eye* mainEye;//視点

	//描画ポインタ
	extern Microsoft::WRL::ComPtr<ID3D12Device> device;
	extern Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
	extern Microsoft::WRL::ComPtr<IDXGIAdapter> useAdapter;
	extern Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

	//命令ポインタ
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	extern Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;
	extern Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;

	//命令エラー防止ポインタ
	extern Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	extern UINT64 fenceValue;
}
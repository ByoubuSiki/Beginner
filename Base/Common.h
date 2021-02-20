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
	//���ʕϐ�

	class Eye;
	class Base;

	extern HWND beginHwnd;
	extern std::list<Base*> outputObject;//�`��ΏۃI�u�W�F�N�g
	extern Eye* mainEye;//���_

	//�`��|�C���^
	extern Microsoft::WRL::ComPtr<ID3D12Device> device;
	extern Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
	extern Microsoft::WRL::ComPtr<IDXGIAdapter> useAdapter;
	extern Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

	//���߃|�C���^
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	extern Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;
	extern Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;

	//���߃G���[�h�~�|�C���^
	extern Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	extern UINT64 fenceValue;
}
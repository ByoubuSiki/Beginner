#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	//GPUに対する命令を管理
	class DirectCommand
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;

		bool CreateAllocator(const Microsoft::WRL::ComPtr<ID3D12Device>&);//コマンドアロケータの初期化
		bool CreateGraphicsList(const Microsoft::WRL::ComPtr<ID3D12Device>&);//コマンドリストの初期化
		bool CreateQueue(const Microsoft::WRL::ComPtr<ID3D12Device>&);//コマンドキューの作成

	public:
		DirectCommand();
		~DirectCommand();

		bool CreateCommand(const Microsoft::WRL::ComPtr<ID3D12Device>&);//メンバを初期化

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetAllocator();
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue();
	};

}
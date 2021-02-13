#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	//GPU�ɑ΂��閽�߂��Ǘ�
	class DirectCommand
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;

		bool CreateAllocator(const Microsoft::WRL::ComPtr<ID3D12Device>&);//�R�}���h�A���P�[�^�̏�����
		bool CreateGraphicsList(const Microsoft::WRL::ComPtr<ID3D12Device>&);//�R�}���h���X�g�̏�����
		bool CreateQueue(const Microsoft::WRL::ComPtr<ID3D12Device>&);//�R�}���h�L���[�̍쐬

	public:
		DirectCommand();
		~DirectCommand();

		bool CreateCommand(const Microsoft::WRL::ComPtr<ID3D12Device>&);//�����o��������

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetAllocator();
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue();
	};

}
#pragma once

#include"..\Base\Pch.h"
#include"DescriptorHeap.h"

namespace Beginner
{

	//DirectX�̕`����Ǘ�
	class DirectGraphics
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Device> device;
		Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> useAdapter;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

		bool CreateDevice();//�f�o�C�X�̏�����
		bool CreateDXGIFactory();//DXGI�̊Ǘ��I�u�W�F�N�g��������
		bool GetUseAdapter();//�g�p�ł���A�_�v�^�̎擾

		bool CreateSwapChain(const Microsoft::WRL::ComPtr<ID3D12CommandQueue>&);//�X���b�v�`�F�C���̍쐬

		UINT GetBackBufferSize();//�o�b�N�o�b�t�@����Ԃ�
		//RTV�Ƃ̊֘A�t��
		bool CreateBackBuffer(std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>&, D3D12_CPU_DESCRIPTOR_HANDLE);

	public:
		DirectGraphics() :device(nullptr),dxgiFactory(nullptr),useAdapter(nullptr),swapChain(nullptr){}
		~DirectGraphics() {}

		bool CreateUntilAdapter();//�A�_�v�^�̎擾�܂ł̏�����

		bool CreateUntilEnd(//�c�������������s��
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
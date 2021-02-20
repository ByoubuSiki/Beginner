#include"DirectGraphics.h"

namespace Beginner
{

	//DirectX�f�o�C�X�̏�����
	bool CreateDevice()
	{
		//�g����o�[�W�������J��Ԃ����ׂ�
		std::vector<D3D_FEATURE_LEVEL> featureList =
		{
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
			D3D_FEATURE_LEVEL_1_0_CORE
		};

		for (unsigned short i = 0; i < featureList.size(); i++)
		{
			HRESULT result = D3D12CreateDevice(//�f�o�C�X�̍쐬
				nullptr, featureList[i], IID_PPV_ARGS(device.ReleaseAndGetAddressOf())
			);

			//�f�o�C�X�̍쐬������
			if (SUCCEEDED(result))
			{
				return true;
			}
		}

		DebugLogOnConsole("�f�o�C�X�̍쐬�����s\n");
		return false;
	}

	//DXGI�̏�����
	bool CreateDXGIFactory()
	{
#ifdef _DEBUG
		HRESULT result = CreateDXGIFactory2(//Error��\������
			DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf())
		);
#else
		//Error��\�������Ȃ�
		HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf()));
#endif // _DEBUG

		//DXGI�̍쐬������
		if (SUCCEEDED(result))
		{
			return true;
		}

		DebugLogOnConsole("DXGIFactory�̍쐬�����s\n");
		return false;
	}

	//�g�p����A�_�v�^�̎擾
	bool GetUseAdapter()
	{
		HRESULT result = dxgiFactory->EnumAdapters(0, useAdapter.ReleaseAndGetAddressOf());

		//�g�p�ł���A�_�v�^�����݂��Ȃ�
		if (FAILED(result))
		{
			DebugLogOnConsole("�A�_�v�^�[�̎擾�����s\n");
			return false;
		}

		return true;
	}

	//1�i�K�ڂ̏�����
	bool CreateUntilAdapter()
	{
		//�A�_�v�^�̍쐬�܂ł�����
		if (CreateDevice() && CreateDXGIFactory() && GetUseAdapter())
		{
			return true;
		}

		return false;
	}

	//�X���b�v�`�F�C���̍쐬
	bool CreateSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};//�X���b�v�`�F�C���̐ݒ�
		{
			swapchainDesc.Width = (UINT)GetWindowSize().x;
			swapchainDesc.Height = (UINT)GetWindowSize().y;
			swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapchainDesc.Stereo = false;
			swapchainDesc.SampleDesc.Count = 1;
			swapchainDesc.SampleDesc.Quality = LOW_QUALITY;
			swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
			swapchainDesc.BufferCount = 2;
			swapchainDesc.Scaling = DXGI_SCALING_STRETCH;//�g�k����
			swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//�t���b�v��͂����ɔj��
			swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//�E�B���h�E�ƃt���X�N���[���̐؂�ւ��\
		}

		HRESULT result = dxgiFactory->CreateSwapChainForHwnd(//DXGI�I�u�W�F�N�g�̍쐬
			cmdQueue.Get(), beginHwnd, &swapchainDesc, nullptr, nullptr,
			(IDXGISwapChain1**)swapChain.ReleaseAndGetAddressOf()
		);

		//�X���b�v�`�F�C���̍쐬�����s
		if (FAILED(result))
		{
			DebugLogOnConsole("�X���b�v�`�F�C���̍쐬�����s\n");
			return false;
		}

		return true;
	}

	//�o�b�N�o�b�t�@����Ԃ�
	UINT GetBackBufferSize()
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		HRESULT result = swapChain->GetDesc(&swapChainDesc);//�X���b�v�`�F�C���̐ݒ���擾

		if (FAILED(result))
		{
			DebugLogOnConsole("�X���b�v�`�F�C���̐ݒ肪�擾�ł��܂���ł�\n");
			return 0;
		}

		return swapChainDesc.BufferCount;
	}

	//RTV�̃o�b�t�@�������֘A�t��
	bool CreateBackBuffer(
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& backBuffer,
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle)
	{
		//�o�b�t�@�T�C�Y�ύX
		const UINT backBufferSize = GetBackBufferSize();
		backBuffer.resize((size_t)backBufferSize);

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		{
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		}

		for (unsigned int index = 0; index < backBuffer.size(); index++)
		{
			//�֘A������o�b�t�@���擾
			HRESULT result = swapChain->GetBuffer(
				index, IID_PPV_ARGS(backBuffer[index].ReleaseAndGetAddressOf())
			);

			if (FAILED(result))
			{
				DebugLogOnConsole("�X���b�v�`�F�C���̃o�b�t�@�擾�����s\n");
				return false;
			}

			//�֘A�t�����Ď��̃A�h���X�ֈړ�
			device->CreateRenderTargetView(backBuffer[index].Get(), &rtvDesc, cpuHandle);
			cpuHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}

		return true;
	}

	//�c�������������s��
	bool CreateUntilEnd(
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& backBuffer,
		const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle
	)
	{
		//�X���b�v�`�F�C���̍쐬�ƃo�b�t�@�Ƃ̊֘A�t��
		if (CreateSwapChain() && CreateBackBuffer(backBuffer, cpuHandle))
		{
			return true;
		}

		return false;
	}
}
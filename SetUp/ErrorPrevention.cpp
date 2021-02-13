#include"ErrorPrevention.h"

namespace Beginner
{

	Prevention::Prevention()
		:fence(nullptr),
		fenceValue(0)
	{

	}

	Prevention::~Prevention()
	{

	}

	//�t�F���X�쐬
	bool Prevention::CreateFence(const Microsoft::WRL::ComPtr<ID3D12Device>& device)
	{
		HRESULT result = device->CreateFence(//�t�F���X�̍쐬
			fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("�t�F���X�̍쐬�����s\n");
			return false;
		}

		return true;
	}

	//GPU�������I���܂ő҂�
	void Prevention::WaitGPUCommandEnd(const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& queue)
	{
		queue->Signal(fence.Get(), ++fenceValue);

		//GPU�̏������I���܂ő҂�
		if (fence->GetCompletedValue() != fenceValue)
		{
			//�C�x���g�̍쐬
			HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);

			fence->SetEventOnCompletion(fenceValue, eventHandle);

			if (eventHandle == 0)
			{
				return;
			}

			//�C�x���g�����܂Œ�~
			WaitForSingleObject(eventHandle, INFINITE);

			CloseHandle(eventHandle);//�C�x���g�I��
		}
	}


	//�����_�[�^�[�Q�b�g�p���\�[�X�o���A
	void Prevention::RenderTargetBarrier(
		const Microsoft::WRL::ComPtr<ID3D12Resource>& resource,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdGraphicsList
	)
	{
		D3D12_RESOURCE_BARRIER barrier = {};//���\�[�X�o���A�̐ݒ�

		{
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = resource.Get();
			barrier.Transition.Subresource = 0;

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		}

		cmdGraphicsList->ResourceBarrier(1, &barrier);//�o���A�����s
	}

	//�t���b�v���\�[�X�o���A
	void Prevention::PresentBarrier(
		const Microsoft::WRL::ComPtr<ID3D12Resource>& resource,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdGraphicsList
	)
	{
		//���\�[�X�o���A�̐ݒ�
		D3D12_RESOURCE_BARRIER barrier = {};

		{
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = resource.Get();
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		}

		cmdGraphicsList->ResourceBarrier(1, &barrier);//�o���A��K�p
	}

	//Region���̃o���A
	void Prevention::RegionBarrier(
		const Microsoft::WRL::ComPtr<ID3D12Resource>& resource,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList
	)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		{
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = resource.Get();
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		}

		cmdList->ResourceBarrier(1, &barrier);
	}

	Microsoft::WRL::ComPtr<ID3D12Fence> Prevention::GetFence()
	{
		return fence;
	}

	//�f�o�b�O�p�̃��C���[��L����
	bool EnableDebugLayer()
	{
#ifdef _DEBUG

		//�f�o�b�O���C���[���擾
		Microsoft::WRL::ComPtr<ID3D12Debug> debugLayer = nullptr;
		HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.ReleaseAndGetAddressOf()));

		if (FAILED(result))
		{
			DebugLogOnConsole("�f�o�b�O���C���[�̎擾�����s\n");
			return false;
		}

		//�f�o�b�O���C���[���N��
		debugLayer->EnableDebugLayer();

#else
		DebugLogOnConsole("Debug���[�h�ł͂Ȃ��̂ŁA�f�o�b�O���[�h�͋N�����܂���\n");
#endif

		return true;
	}
}
#include"Begin.h"

namespace Beginner
{

	//DirectX�̏�����
	bool Begin::Init(const HWND hwnd)
	{
		beginHwnd = hwnd;

		//DirectX�̃G���[�������E�������E�ݒ�
		if (!init && EnableDebugLayer() && DeviceCommandInits() && BufferInits())
		{
			init = true;//�t���O���Ă�

			CreateDepthView(depthBuffer, dsvHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());

			return true;
		}

		DebugLogOnConsole("Begin�N���X�̏����������s\n");
		return false;
	}

	// �����_�[�^�[�Q�b�g�̎w��E��ʃN���A
	void Begin::FlameStart()
	{
		//�o���A�`���Ԃɐݒ�
		const UINT bufferIndex = swapChain->GetCurrentBackBufferIndex();
		RenderTargetBarrier(rtBuffer[bufferIndex]);

		//RT�̃A�h���X����RTV��ݒ�
		const auto rtvHandle = GetRTVAddress();
		const auto dsvView = dsvHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart();
		commandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvView);

		//��ʃN���A
		float color[] = { 1.0F,1.0F,1.0F,1.0F };
		commandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
		commandList->ClearDepthStencilView(dsvView, D3D12_CLEAR_FLAG_DEPTH, 1.0F, 0, 0, nullptr);
	}

	// ���߂������E��ʃt���b�v
	bool Begin::FlameEnd()
	{
		Draw();	//�`�揈��

		//������Ԃ�ω�
		const UINT bufferIndex = swapChain->GetCurrentBackBufferIndex();
		PresentBarrier(rtBuffer[bufferIndex]);

		//���߂̏����Ɏ��s
		if (!CommandAction())
		{
			DebugLogOnConsole("���߂̏��������s\n");
			return false;
		}

		//��ʃt���b�v
		if (FAILED(swapChain->Present(1, 0)))
		{
			DebugLogOnConsole("�t���b�v���s\n");
			return false;
		}

		return true;
	}

	// RTV�̃A�h���X�擾
	D3D12_CPU_DESCRIPTOR_HANDLE Begin::GetRTVAddress()
	{
		//�o�b�N�o�b�t�@�̎擾�Ɏg�p
		const UINT bufferIndex = swapChain->GetCurrentBackBufferIndex();

		//RTV�f�B�X�N���v�^�̃A�h���X���ʒu��������
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart();
		const UINT rtvIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		rtvHandle.ptr += bufferIndex * rtvIncrementSize;

		return rtvHandle;
	}

	//�o�^�ς݂̃I�u�W�F�N�g��`��
	void Begin::Draw()
	{
		mainEye->UpdateViewMatrix();//���_�����X�V

		//�`��͈͂�ݒ�
		DrawRange drawRange = { GetWindowSize() };

		commandList->RSSetScissorRects(1, &drawRange.GetRect());
		commandList->RSSetViewports(1, &drawRange.GetViewPort());

		//�I�u�W�F�N�g�̕`�揈�����s��
		for (auto registItr = outputObject.begin(); registItr != outputObject.end(); registItr++)
		{
			(*registItr)->DrawCall();//�`��Ώۂ��Ƃɏ������Ăяo��
		}
	}

	// ���[�U�[������̖��߂�����
	bool Begin::CommandAction()
	{
		//���߂̎�t���I��
		if (FAILED(commandList->Close()))
		{
			DebugLogOnConsole("���߂̃N���[�Y���������s\n");
			return false;
		}

		//���߂����s ���������܂Œ�~
		ID3D12CommandList* commandLists[] = { commandList.Get() };
		cmdQueue->ExecuteCommandLists(1, commandLists);
		WaitGPUCommandEnd();

		//���߂���ɂ���
		if (FAILED(allocator->Reset()))
		{
			DebugLogOnConsole("���߃A���P�[�^�̃��Z�b�g�����s\n");
			return false;
		}

		//�R�}���h���X�g�����Z�b�g
		if (FAILED(commandList->Reset(allocator.Get(), nullptr)))
		{
			DebugLogOnConsole("�R�}���h���X�g�̃��Z�b�g�����s\n");
			return false;
		}

		return true;
	}

	// �N���X�̏��������܂Ƃ߂�
	bool Begin::DeviceCommandInits()
	{
		//�`��E���߃I�u�W�F�N�g��������
		if (CreateUntilAdapter() &&
			CreateCommand() &&
			rtvHeap.CreateRTV(2) &&
			CreateUntilEnd(rtBuffer, rtvHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart()))
		{

			return true;
		}

		return false;
	}

	//Buffer��View�̏�����
	bool Begin::BufferInits()
	{
		if (dsvHeap.CreateDSV() && CreateDepthBuffer(depthBuffer) && CreateFence())
		{
			return true;
		}

		return false;
	}
}
#include"DirectCommand.h"

namespace Beginner
{

	//���߃I�u�W�F�N�g�̏�����
	bool CreateCommand()
	{
		//���ߌn���̏�����
		if (CreateAllocator() && CreateGraphicsList() && CreateQueue())
		{
			return true;
		}

		return false;
	}

	//�R�}���h�A���P�[�^�̏�����
	bool CreateAllocator()
	{
		HRESULT result = device->CreateCommandAllocator(//�A���P�[�^���쐬
			D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(allocator.ReleaseAndGetAddressOf())
		);

		//�쐬�����s
		if (FAILED(result))
		{
			DebugLogOnConsole("�R�}���h�A���P�[�^�̏����������s\n");
			return false;
		}

		return true;
	}

	//�R�}���h���X�g�̍쐬
	bool CreateGraphicsList()
	{
		HRESULT result = device->CreateCommandList(//�R�}���h���X�g���쐬
			0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			allocator.Get(), nullptr, IID_PPV_ARGS(commandList.ReleaseAndGetAddressOf())
		);

		//�R�}���h���X�g�̍쐬�����s
		if (FAILED(result))
		{
			DebugLogOnConsole("�O���t�B�b�N�X�R�}���h���X�g�̏����������s\n");
			return false;
		}

		return true;
	}

	//�R�}���h�L���[�̏�����
	bool CreateQueue()
	{
		//�R�}���h�L���[�ݒ�
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};

		{
			cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			cmdQueueDesc.NodeMask = 0;
			cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		}

		HRESULT result = device->CreateCommandQueue(//�R�}���h�L���[��ݒ�
			&cmdQueueDesc, IID_PPV_ARGS(cmdQueue.ReleaseAndGetAddressOf())
		);

		//�R�}���h�L���[�̍쐬�����s
		if (FAILED(result))
		{
			DebugLogOnConsole("�R�}���h�L���[�̍쐬�����s\n");
			return false;
		}

		return true;
	}

}
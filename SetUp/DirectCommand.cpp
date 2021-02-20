#include"DirectCommand.h"

namespace Beginner
{

	//命令オブジェクトの初期化
	bool CreateCommand()
	{
		//命令系統の初期化
		if (CreateAllocator() && CreateGraphicsList() && CreateQueue())
		{
			return true;
		}

		return false;
	}

	//コマンドアロケータの初期化
	bool CreateAllocator()
	{
		HRESULT result = device->CreateCommandAllocator(//アロケータを作成
			D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(allocator.ReleaseAndGetAddressOf())
		);

		//作成が失敗
		if (FAILED(result))
		{
			DebugLogOnConsole("コマンドアロケータの初期化が失敗\n");
			return false;
		}

		return true;
	}

	//コマンドリストの作成
	bool CreateGraphicsList()
	{
		HRESULT result = device->CreateCommandList(//コマンドリストを作成
			0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			allocator.Get(), nullptr, IID_PPV_ARGS(commandList.ReleaseAndGetAddressOf())
		);

		//コマンドリストの作成が失敗
		if (FAILED(result))
		{
			DebugLogOnConsole("グラフィックスコマンドリストの初期化が失敗\n");
			return false;
		}

		return true;
	}

	//コマンドキューの初期化
	bool CreateQueue()
	{
		//コマンドキュー設定
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};

		{
			cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			cmdQueueDesc.NodeMask = 0;
			cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		}

		HRESULT result = device->CreateCommandQueue(//コマンドキューを設定
			&cmdQueueDesc, IID_PPV_ARGS(cmdQueue.ReleaseAndGetAddressOf())
		);

		//コマンドキューの作成が失敗
		if (FAILED(result))
		{
			DebugLogOnConsole("コマンドキューの作成が失敗\n");
			return false;
		}

		return true;
	}

}
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

	//フェンス作成
	bool Prevention::CreateFence(const Microsoft::WRL::ComPtr<ID3D12Device>& device)
	{
		HRESULT result = device->CreateFence(//フェンスの作成
			fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf())
		);

		if (FAILED(result))
		{
			DebugLogOnConsole("フェンスの作成が失敗\n");
			return false;
		}

		return true;
	}

	//GPU処理が終わるまで待つ
	void Prevention::WaitGPUCommandEnd(const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& queue)
	{
		queue->Signal(fence.Get(), ++fenceValue);

		//GPUの処理が終わるまで待つ
		if (fence->GetCompletedValue() != fenceValue)
		{
			//イベントの作成
			HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);

			fence->SetEventOnCompletion(fenceValue, eventHandle);

			if (eventHandle == 0)
			{
				return;
			}

			//イベント発生まで停止
			WaitForSingleObject(eventHandle, INFINITE);

			CloseHandle(eventHandle);//イベント終了
		}
	}


	//レンダーターゲット用リソースバリア
	void Prevention::RenderTargetBarrier(
		const Microsoft::WRL::ComPtr<ID3D12Resource>& resource,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdGraphicsList
	)
	{
		D3D12_RESOURCE_BARRIER barrier = {};//リソースバリアの設定

		{
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = resource.Get();
			barrier.Transition.Subresource = 0;

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		}

		cmdGraphicsList->ResourceBarrier(1, &barrier);//バリアを実行
	}

	//フリップリソースバリア
	void Prevention::PresentBarrier(
		const Microsoft::WRL::ComPtr<ID3D12Resource>& resource,
		const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdGraphicsList
	)
	{
		//リソースバリアの設定
		D3D12_RESOURCE_BARRIER barrier = {};

		{
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = resource.Get();
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		}

		cmdGraphicsList->ResourceBarrier(1, &barrier);//バリアを適用
	}

	//Region時のバリア
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

	//デバッグ用のレイヤーを有効化
	bool EnableDebugLayer()
	{
#ifdef _DEBUG

		//デバッグレイヤーを取得
		Microsoft::WRL::ComPtr<ID3D12Debug> debugLayer = nullptr;
		HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.ReleaseAndGetAddressOf()));

		if (FAILED(result))
		{
			DebugLogOnConsole("デバッグレイヤーの取得が失敗\n");
			return false;
		}

		//デバッグレイヤーを起動
		debugLayer->EnableDebugLayer();

#else
		DebugLogOnConsole("Debugモードではないので、デバッグモードは起動しません\n");
#endif

		return true;
	}
}
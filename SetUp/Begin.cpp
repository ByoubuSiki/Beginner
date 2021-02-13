#include"Begin.h"

namespace Beginner
{

	//DirectXの初期化
	bool Begin::Init(const HWND hwnd)
	{
		winHandle = hwnd;

		//DirectXのエラーを可視化・初期化・設定
		if (!init && EnableDebugLayer() && DeviceCommandInits() && BufferInits())
		{

			init = true;//フラグ建てる

			CreateDepthView(winHandle, depthBuffer, dsvHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());

			return true;
		}

		DebugLogOnConsole("Beginクラスの初期化が失敗\n");
		return false;
	}

	// レンダーターゲットの指定・画面クリア
	void Begin::FlameStart()
	{
		//バリア描画状態に設定
		const UINT bufferIndex = graphics.GetSwapChain()->GetCurrentBackBufferIndex();
		prevention.RenderTargetBarrier(rtBuffer[bufferIndex], commandList);

		//RTのアドレスからRTVを設定
		const auto rtvHandle = GetRTVAddress();
		const auto dsvView = dsvHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart();
		commandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvView);

		//画面クリア
		float color[] = { 1.0F,1.0F,1.0F,1.0F };
		commandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);

		commandList->ClearDepthStencilView(dsvView, D3D12_CLEAR_FLAG_DEPTH, 1.0F, 0, 0, nullptr);
	}

	// 命令を処理・画面フリップ
	bool Begin::FlameEnd()
	{
		Draw();	//描画処理

		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain = graphics.GetSwapChain();

		//処理状態を変化
		const UINT bufferIndex = swapChain->GetCurrentBackBufferIndex();
		prevention.PresentBarrier(rtBuffer[bufferIndex], commandList);

		//命令の処理に失敗
		if (!CommandAction())
		{
			DebugLogOnConsole("命令の処理が失敗\n");
			return false;
		}

		//画面フリップ
		if (FAILED(swapChain->Present(1, 0)))
		{
			DebugLogOnConsole("フリップ失敗\n");
			return false;
		}

		return true;
	}

	// 表示オブジェクトの登録
	bool Begin::Regist(Base* base)
	{
		if (base == nullptr)
		{
			DebugLogOnConsole("Regist対象がnullptr\n");
			return false;
		}

		//Regist済みである
		if (base->GetRegist())
		{
			DebugLogOnConsole("Regist済み\n");
			return false;
		}

		//登録時の動作を判定
		if (!base->RegistCall(winHandle))
		{
			DebugLogOnConsole("Registの失敗\n");
			return false;
		}

		//RegistCallの命令を処理待ち
		if (!CommandAction())
		{
			DebugLogOnConsole("Regist時のCommandProcessが失敗\n");
			return false;
		}

		registBase.push_back(base);//Registしたオブジェクトを記録

		return true;
	}

	//視点オブジェクトの登録
	bool Begin::Regist(Eye* eye)
	{
		if (eye == nullptr)
		{
			DebugLogOnConsole("Regist対象がnullptr\n");
			return false;
		}

		//Regist済みである
		if (eye->GetRegist())
		{
			DebugLogOnConsole("Regist済み\n");
			return false;
		}

		mainEye = eye;

		return true;
	}

	// RTVのアドレス取得
	D3D12_CPU_DESCRIPTOR_HANDLE Begin::GetRTVAddress()
	{
		//バックバッファの取得に使用
		const UINT bufferIndex = graphics.GetSwapChain()->GetCurrentBackBufferIndex();

		//RTVディスクリプタのアドレスを位置調整する
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart();
		const UINT rtvIncrementSize = graphics.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		rtvHandle.ptr += bufferIndex * rtvIncrementSize;

		return rtvHandle;
	}

	//登録済みのオブジェクトを描画
	void Begin::Draw()
	{
		mainEye->UpdateViewMatrix(winHandle);//視点情報を更新

		//描画範囲を設定
		DrawRange drawRange = { GetWindowSize(winHandle) };

		commandList->RSSetScissorRects(1, &drawRange.GetRect());
		commandList->RSSetViewports(1, &drawRange.GetViewPort());

		//オブジェクトの描画処理を行う
		for (auto registItr = registBase.begin();
			registItr != registBase.end();
			registItr++)
		{
			(*registItr)->DrawCall(winHandle);//描画対象ごとに処理を呼び出し
		}
	}

	// ユーザー側からの命令を処理
	bool Begin::CommandAction()
	{
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator = command.GetAllocator();
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue = command.GetCommandQueue();

		//命令の受付を終了
		if (FAILED(commandList->Close()))
		{
			DebugLogOnConsole("命令のクローズ処理が失敗\n");
			return false;
		}

		//命令を実行 処理完了まで停止
		ID3D12CommandList* commandLists[] = { commandList.Get() };
		queue->ExecuteCommandLists(1, commandLists);
		prevention.WaitGPUCommandEnd(queue);

		//命令を空にする
		if (FAILED(allocator->Reset()))
		{
			DebugLogOnConsole("命令アロケータのリセットが失敗\n");
			return false;
		}

		//コマンドリストをリセット
		if (FAILED(commandList->Reset(allocator.Get(), nullptr)))
		{
			DebugLogOnConsole("コマンドリストのリセットが失敗\n");
			return false;
		}

		return true;
	}

	// クラスの初期化をまとめる
	bool Begin::DeviceCommandInits()
	{
		//描画・命令オブジェクトを初期化
		if (graphics.CreateUntilAdapter() &&
			command.CreateCommand(graphics.GetDevice()) &&
			rtvHeap.CreateRTV(graphics.GetDevice(), 2) &&
			graphics.CreateUntilEnd(command.GetCommandQueue(), rtBuffer, rtvHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart(), winHandle))
		{
			device = graphics.GetDevice();
			commandList = command.GetGraphicsCommandList();
			return true;
		}

		return false;
	}

	//BufferとViewの初期化
	bool Begin::BufferInits()
	{
		if (dsvHeap.CreateDSV(graphics.GetDevice()) && CreateDepthBuffer(winHandle, depthBuffer) && prevention.CreateFence(graphics.GetDevice()))
		{
			return true;
		}

		return false;
	}
}
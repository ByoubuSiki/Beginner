#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	//フェンスとバリアの機能を持つ
	class Prevention
	{
	public:
		Prevention();
		~Prevention();

		bool CreateFence(const Microsoft::WRL::ComPtr<ID3D12Device>&);//フェンス初期化
		void WaitGPUCommandEnd(const Microsoft::WRL::ComPtr<ID3D12CommandQueue>&);//GPU処理が終わるまで待つ

		void RenderTargetBarrier(//レンダーターゲットバリア
			const Microsoft::WRL::ComPtr<ID3D12Resource>&,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&
		);

		void PresentBarrier(//フリップバリア
			const Microsoft::WRL::ComPtr<ID3D12Resource>&,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&
		);

		void RegionBarrier(//Region時のバリア
			const Microsoft::WRL::ComPtr<ID3D12Resource>&,
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&
		);

		Microsoft::WRL::ComPtr<ID3D12Fence> GetFence();

	private:
		Microsoft::WRL::ComPtr<ID3D12Fence> fence;//フェンスオブジェクト
		UINT64 fenceValue;
	};

	//デバッグ用のレイヤーを有効化
	bool EnableDebugLayer();
}